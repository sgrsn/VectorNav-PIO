// The MIT License (MIT)
// 
// VectorNav SDK (v0.19.0)
// Copyright (c) 2024 VectorNav Technologies, LLC
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "Implementation/PacketSynchronizer.hpp"
#include "Debug.hpp"

namespace VN
{

bool PacketSynchronizer::addDispatcher(PacketDispatcher* packetParser) noexcept
{
    return _dispatchers.push_back({packetParser, packetParser->getSyncBytes(), PacketDispatcher::FindPacketRetVal()});
}

bool PacketSynchronizer::dispatchNextPacket() noexcept
{
    size_t byteBufferSize = _primaryByteBuffer.size();
    if (byteBufferSize == 0 || ((_prevValidity == PacketDispatcher::FindPacketRetVal::Validity::Incomplete) && (byteBufferSize < _prevBytesRequested)))
    {
        // Early return if there's no new data
        return true;
    }
    _prevByteBufferSize = byteBufferSize;
    VN_PROFILER_TIME_CURRENT_SCOPE();
    for (size_t fromHeadIndex = 0; fromHeadIndex < byteBufferSize; ++fromHeadIndex)
    {
        for (const auto& currentDispatcher : this->_dispatchers)
        {
            // TODO 133: Modify to handle multi-size sync bytes
            if (currentDispatcher.syncBytes.front() == _primaryByteBuffer.peek_unchecked(fromHeadIndex))
            {
                auto retVal = currentDispatcher.packetDispatcher->findPacket(_primaryByteBuffer, fromHeadIndex);
                switch (retVal.validity)
                {
                    case (PacketDispatcher::FindPacketRetVal::Validity::Valid):
                    {
                        ++currentDispatcher.numValidPackets;
                        VN_DEBUG_2("Packet found: " + std::to_string(currentDispatcher.syncBytes.front()) + " length: " + std::to_string(retVal.length));
                        currentDispatcher.packetDispatcher->dispatchPacket(_primaryByteBuffer, fromHeadIndex);

                        // Require that at least the sync bytes are discarded, to prevent locking due to a bad dispatcher
                        size_t numPacketBytesToDiscard = std::max(currentDispatcher.syncBytes.size(), retVal.length);
                        _copyToSkippedByteBufferIfEnabled(fromHeadIndex);
                        _copyToReceivedByteBufferIfEnabled(fromHeadIndex + numPacketBytesToDiscard);
                        _primaryByteBuffer.discard(fromHeadIndex + numPacketBytesToDiscard);

                        // We are returning so that we can pull data off the serial queue after each packet. This way we don't blow through the whole buffer
                        // at a time while the serial queue overflows.
                        _prevValidity = PacketDispatcher::FindPacketRetVal::Validity::Valid;
                        _prevByteBufferSize -= fromHeadIndex + numPacketBytesToDiscard;
                        return false;
                    }
                    case (PacketDispatcher::FindPacketRetVal::Validity::Invalid):
                    {
                        // Keep searching, might have just been a random sync byte.
                        ++currentDispatcher.numInvalidPackets;
                        continue;
                    }
                    case (PacketDispatcher::FindPacketRetVal::Validity::Incomplete):
                    {
                        // Let's trust that this is probably a packet of this type, so we'll wait for more data and start searching again.
                        // We might as well discard all of the bytes so far, because clearly no one wanted it.
                        VN_DEBUG_2("Found possible packet: " + std::to_string(currentDispatcher.syncBytes.front()) +
                                   " bytes available: " + std::to_string(_primaryByteBuffer.size()));

                        // "About to overrun" is roughly true if the bytes avaialble to this packet dispatcher is within 1 serial push of being full. If that's
                        // true and the dispatcher is returning "incomplete", it's probably never going to find it's packet and is being too greedy. Instead we
                        // should continue and let the other dispatchers search for packets.
                        bool aboutToOverrun = (_primaryByteBuffer.capacity() - (byteBufferSize - fromHeadIndex)) < _nominalSerialPush;
                        if (aboutToOverrun) { continue; }

                        _copyToSkippedByteBufferIfEnabled(fromHeadIndex);
                        _copyToReceivedByteBufferIfEnabled(fromHeadIndex);
                        _primaryByteBuffer.discard(fromHeadIndex);
                        _prevByteBufferSize -= fromHeadIndex;
                        _prevValidity = PacketDispatcher::FindPacketRetVal::Validity::Incomplete;
                        _prevBytesRequested = retVal.length;
                        return true;
                    }
                    default:
                        VN_ABORT();
                }
            }
        }
    }
    // At this point, we can flush the buffer, because no one is interested in any of the data.
    _copyToSkippedByteBufferIfEnabled(byteBufferSize);
    _copyToReceivedByteBufferIfEnabled(byteBufferSize);
    _primaryByteBuffer.discard(byteBufferSize);
    _prevByteBufferSize -= byteBufferSize;
    _prevValidity = PacketDispatcher::FindPacketRetVal::Validity::Invalid;
    return true;
}

size_t PacketSynchronizer::getValidPacketCount(const SyncBytes& syncBytes) const noexcept
{
    for (auto dispatcher : _dispatchers)
    {
        if (syncBytes == dispatcher.syncBytes) { return dispatcher.numValidPackets; }
    }
    return 0;
}

size_t PacketSynchronizer::getInvalidPacketCount(const SyncBytes& syncBytes) const noexcept
{
    for (auto dispatcher : _dispatchers)
    {
        if (syncBytes == dispatcher.syncBytes) { return dispatcher.numInvalidPackets; }
    }
    return 0;
}

void PacketSynchronizer::_copyToSkippedByteBufferIfEnabled(const size_t numBytesToCopy) const noexcept
{
    if (numBytesToCopy == 0) { return; }
    _skippedByteCount += numBytesToCopy;
    VN_DEBUG_2("Discovered skipped bytes: " + std::to_string(numBytesToCopy));
    if (_pSkippedByteBuffer)
    {
        size_t bytesRemainingToCopy = numBytesToCopy;
        size_t bytesCopied = 0;
        do {
            const size_t bytesToCopy = std::min(bytesRemainingToCopy, _copySkippedReceivedLinearBuffer.size());
            if (_primaryByteBuffer.peek(_copySkippedReceivedLinearBuffer.data(), bytesToCopy, bytesCopied)) { VN_ABORT(); }
            if (_pSkippedByteBuffer->put(_copySkippedReceivedLinearBuffer.data(), bytesToCopy))
            {
                if (_asyncErrorQueuePush) { _asyncErrorQueuePush(AsyncError(Error::SkippedByteBufferFull)); }
                return;
            }
            bytesRemainingToCopy -= bytesToCopy;
            bytesCopied += bytesToCopy;
        } while (bytesRemainingToCopy > 0);
    }
}

void PacketSynchronizer::_copyToReceivedByteBufferIfEnabled(const size_t numBytesToCopy) const noexcept
{
    if (numBytesToCopy == 0) { return; }
    _receivedByteCount += numBytesToCopy;
    if (_pReceivedByteBuffer)
    {
        size_t bytesRemainingToCopy = numBytesToCopy;
        size_t bytesCopied = 0;
        do {
            const size_t bytesToCopy = std::min(bytesRemainingToCopy, _copySkippedReceivedLinearBuffer.size());
            if (_primaryByteBuffer.peek(_copySkippedReceivedLinearBuffer.data(), bytesToCopy, bytesCopied)) { VN_ABORT(); }
            if (_pReceivedByteBuffer->put(_copySkippedReceivedLinearBuffer.data(), bytesToCopy))
            {
                if (_asyncErrorQueuePush) { _asyncErrorQueuePush(AsyncError(Error::ReceivedByteBufferFull)); }
                return;
            }
            bytesRemainingToCopy -= bytesToCopy;
            bytesCopied += bytesToCopy;
        } while (bytesRemainingToCopy > 0);
    }
}
}  // namespace VN
