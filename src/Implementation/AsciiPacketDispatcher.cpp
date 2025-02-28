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

#include "Implementation/AsciiPacketDispatcher.hpp"
#include "Implementation/AsciiPacketProtocol.hpp"

namespace VN
{
PacketDispatcher::FindPacketRetVal AsciiPacketDispatcher::findPacket(const ByteBuffer& byteBuffer, const size_t syncByteIndex) noexcept
{
    const AsciiPacketProtocol::FindPacketReturn findPacketRetVal = AsciiPacketProtocol::findPacket(byteBuffer, syncByteIndex);
    if (findPacketRetVal.validity == AsciiPacketProtocol::Validity::Valid) { _latestPacketMetadata = findPacketRetVal.metadata; }
    return {findPacketRetVal.validity, findPacketRetVal.metadata.length};
}

void AsciiPacketDispatcher::dispatchPacket(const ByteBuffer& byteBuffer, const size_t syncByteIndex) noexcept
{
    VN_PROFILER_TIME_CURRENT_SCOPE();
    bool packetHasBeenConsumed = false;
    if (StringUtils::startsWith(_latestPacketMetadata.header, "VN"))
    {
        AsciiPacketProtocol::AsciiMeasurementHeader asciiHeader = AsciiPacketProtocol::getMeasHeader(_latestPacketMetadata.header);
        if (asciiHeader != AsciiPacketProtocol::AsciiMeasurementHeader::None)
        {
            if (AsciiPacketProtocol::asciiIsParsable(asciiHeader))
            {
                _invokeSubscribers(byteBuffer, syncByteIndex, _latestPacketMetadata);
                if constexpr (Config::PacketDispatchers::compositeDataQueueCapacity > 0)
                {
                    packetHasBeenConsumed |= _tryPushToCompositeDataQueue(byteBuffer, syncByteIndex, _latestPacketMetadata, asciiHeader);
                }
            }
        }
        else
        {  // Data is not a measurement. This includes vnerrs.
            VN_DEBUG_1("Passing command response.");
            AsciiMessage packet{};
            for (size_t idx = 0; idx < _latestPacketMetadata.length; idx++) { packet.push_back(byteBuffer.peek_unchecked(syncByteIndex + idx)); }
            _commandProcessor->matchResponse(packet, _latestPacketMetadata);
        }
    }
    else
    {  // Data does not begin with "VN"
        _invokeSubscribers(byteBuffer, syncByteIndex, _latestPacketMetadata);
    }
}

bool AsciiPacketDispatcher::addSubscriber(PacketQueue_Interface* subscriber, const AsciiHeader& headerToUse, SubscriberFilterType filterType) noexcept
{
    if (subscriber == nullptr) { return true; }
    if (headerToUse.empty()) { filterType = SubscriberFilterType::StartsWith; }
    return _subscribers.push_back(Subscriber{subscriber, headerToUse, filterType});
}

void AsciiPacketDispatcher::removeSubscriber(PacketQueue_Interface* subscriberToRemove) noexcept
{
    for (auto itr = _subscribers.begin(); itr != _subscribers.end(); ++itr)
    {
        auto& subscriber = *itr;
        if (subscriberToRemove == subscriber.queueToPush) { _subscribers.erase(itr); }
    }
}

void AsciiPacketDispatcher::removeSubscriber(PacketQueue_Interface* subscriberToRemove, const AsciiHeader& headerToUse) noexcept
{
    for (auto itr = _subscribers.begin(); itr != _subscribers.end(); ++itr)
    {
        auto& subscriber = *itr;
        if (subscriberToRemove == subscriber.queueToPush)
        {
            if (headerToUse == subscriber.headerFilter)
            {
                _subscribers.erase(itr);
                break;
            }
        }
    }
}

bool AsciiPacketDispatcher::_tryPushToCompositeDataQueue(const ByteBuffer& byteBuffer, const size_t syncByteIndex,
                                                         const AsciiPacketProtocol::Metadata& metadata,
                                                         AsciiPacketProtocol::AsciiMeasurementHeader measEnum) noexcept
{
    // if (!AsciiPacketProtocol::anyDataIsEnabled(metadata.header, _enabledMeasurements)) { return false; }
    auto compositeData = AsciiPacketProtocol::parsePacket(byteBuffer, syncByteIndex, metadata, measEnum);
    if (!compositeData.has_value()) { return false; }

    // Copy to the output queue
    auto pCompositeData = _compositeDataQueue->put();
    if (!pCompositeData) { return false; }
    *pCompositeData = compositeData.value();  // Todo 477: INvestigate passing pointer into the parser, rather than returning and copying it. Will that
                                              // be more efficient than calling "reset" and assigning values?
    return true;
}

void AsciiPacketDispatcher::_invokeSubscribers(const ByteBuffer& byteBuffer, const size_t syncByteIndex, const AsciiPacketProtocol::Metadata& metadata) noexcept
{
    for (auto& subscriber : _subscribers)
    {
        if (StringUtils::startsWith(metadata.header, subscriber.headerFilter))
        {
            if (subscriber.filterType == SubscriberFilterType::StartsWith)
            {
                [[maybe_unused]] const bool failed = _tryPushToSubscriber(byteBuffer, syncByteIndex, metadata, subscriber);
            }
        }
        else
        {
            if (subscriber.filterType == SubscriberFilterType::DoesNotStartWith)
            {
                [[maybe_unused]] const bool failed = _tryPushToSubscriber(byteBuffer, syncByteIndex, metadata, subscriber);
            }
        }
    }
}

bool AsciiPacketDispatcher::_tryPushToSubscriber(const ByteBuffer& byteBuffer, const size_t syncByteIndex, const AsciiPacketProtocol::Metadata& metadata,
                                                 Subscriber& subscriber) noexcept
{
    auto putSlot = subscriber.queueToPush->put();
    if (putSlot)
    {
        putSlot->details.syncByte = PacketDetails::SyncByte::Ascii;
        putSlot->details.asciiMetadata = metadata;
        byteBuffer.peek_unchecked(putSlot->buffer, metadata.length, syncByteIndex);
    }
    else
    {
        // Putting failed
        return true;
    }
    return false;
}

}  // namespace VN
