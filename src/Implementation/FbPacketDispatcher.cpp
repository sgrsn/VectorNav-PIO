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

#include "Implementation/FbPacketDispatcher.hpp"

namespace VN
{
PacketDispatcher::FindPacketRetVal FbPacketDispatcher::findPacket(const ByteBuffer& byteBuffer, const size_t syncByteIndex) noexcept
{
    FbPacketProtocol::FindPacketReturn findPacketRetVal = FbPacketProtocol::findPacket(byteBuffer, syncByteIndex);
    if (findPacketRetVal.validity == FbPacketProtocol::Validity::Valid) { _latestPacketMetadata = findPacketRetVal.metadata; }
    return {findPacketRetVal.validity, findPacketRetVal.metadata.length};
}

void FbPacketDispatcher::dispatchPacket(const ByteBuffer& byteBuffer, const size_t syncByteIndex) noexcept
{
    // We must assume that _latestPacketMetadata is correctly set.

    const bool packetIsInvalid = (_latestPacketMetadata.header.currentPacketCount == 1)
                                     ? false
                                     : (_latestPacketMetadata.header.messageId != _previousPacketMetadata.header.messageId) ||
                                           ((_latestPacketMetadata.header.messageId == _previousPacketMetadata.header.messageId) &&
                                            (_latestPacketMetadata.header.currentPacketCount != (_previousPacketMetadata.header.currentPacketCount + 1)));
    if (packetIsInvalid)
    {
        // Unless this is the first packet of a message, the packet is invalid if it's either:
        //   1. Isn't the same messageId as the previous
        //   2. The same message as previous, but not next in order (nor the first of a new)
        // We should wipe the copied buffer and move on.
        _fbByteBuffer.reset();
        return;
    }

    if (_latestPacketMetadata.header.currentPacketCount == 1) { _resetFbBuffer(); }

    const bool errorOccured = _moveBytesFromMainBufferToFbBuffer(_latestPacketMetadata.header, byteBuffer, _latestPacketMetadata.header.payloadLength,
                                                                 syncByteIndex + 1 + 5);  // Add after FB header
    if (errorOccured) { return; }

    const bool packetIsFinalOfMessage = _latestPacketMetadata.header.currentPacketCount == _latestPacketMetadata.header.totalPacketCount;
    if (packetIsFinalOfMessage)
    {
        _addFaPacketCrc();

        const auto retVal = _faPacketDispatcher->findPacket(_fbByteBuffer, 0);

        if (retVal.validity == PacketDispatcher::FindPacketRetVal::Validity::Valid) { _faPacketDispatcher->dispatchPacket(_fbByteBuffer, 0); }
        _fbByteBuffer.reset();
    }
    _previousPacketMetadata = _latestPacketMetadata;
}

bool FbPacketDispatcher::_moveBytesFromMainBufferToFbBuffer(SplitPacketDetails splitPacketDetails, const ByteBuffer& byteBuffer, const size_t numOfBytesToMove,
                                                            const size_t startingIndex) noexcept
{
    if (splitPacketDetails.payloadLength > byteBuffer.size()) { return true; }
    for (size_t i = 0; i < numOfBytesToMove; i++)
    {
        auto tmpByte = byteBuffer.peek_unchecked(startingIndex + i);
        _fbByteBuffer.put(&tmpByte, 1);
    }
    return false;
}

void FbPacketDispatcher::_resetFbBuffer() noexcept
{
    _fbByteBuffer.reset();
    const uint8_t faSyncByte = 0xFA;
    _fbByteBuffer.put(&faSyncByte, 1);
}

void FbPacketDispatcher::_addFaPacketCrc() noexcept
{
    uint16_t crc = 0;

    // Calculate a CRC over everything but the sync byte
    for (size_t i = 1; i < _fbByteBuffer.size(); ++i) { _calculateCRC(&crc, _fbByteBuffer.peek(i).value()); }

    // Crc is put in big endian
    uint8_t data = 0;
    data = static_cast<uint8_t>(crc >> 8);
    _fbByteBuffer.put(&data, 1);  // CRC
    data = static_cast<uint8_t>(crc & 0xFF);
    _fbByteBuffer.put(&data, 1);  // CRC
}

}  // namespace VN
