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

#include "Implementation/FaPacketDispatcher.hpp"

namespace VN
{
PacketDispatcher::FindPacketRetVal FaPacketDispatcher::findPacket(const ByteBuffer& byteBuffer, const size_t syncByteIndex) noexcept
{
    FaPacketProtocol::FindPacketReturn findPacketRetVal = FaPacketProtocol::findPacket(byteBuffer, syncByteIndex);
    if (findPacketRetVal.validity == FaPacketProtocol::Validity::Valid) { _latestPacketMetadata = findPacketRetVal.metadata; }
    return {findPacketRetVal.validity, findPacketRetVal.metadata.length};
}

void FaPacketDispatcher::dispatchPacket(const ByteBuffer& byteBuffer, const size_t syncByteIndex) noexcept
{
    VN_PROFILER_TIME_CURRENT_SCOPE();
    bool packetConsumed = false;
    _invokeSubscribers(byteBuffer, syncByteIndex, _latestPacketMetadata);
    if constexpr (Config::PacketDispatchers::compositeDataQueueCapacity > 0)
    {
        packetConsumed |= _tryPushToCompositeDataQueue(byteBuffer, syncByteIndex, _latestPacketMetadata);
    }
}

bool FaPacketDispatcher::addSubscriber(PacketQueue_Interface* subscriber, EnabledMeasurements headerToUse, SubscriberFilterType filterType) noexcept
{
    if (headerToUse == EnabledMeasurements{0})
    {
        // If they pass no header filter, we should match on any message
        for (auto& group : headerToUse) { group = std::numeric_limits<uint32_t>::max(); }
        filterType = SubscriberFilterType::AnyMatch;
    }
    return _subscribers.push_back(Subscriber{subscriber, headerToUse, filterType});
}

void FaPacketDispatcher::removeSubscriber(PacketQueue_Interface* subscriberToRemove) noexcept
{
    for (auto itr = _subscribers.rbegin(); itr != _subscribers.rend(); ++itr)
    {
        auto& subscriber = *itr;
        if (subscriberToRemove == subscriber.queueToPush) { _subscribers.erase(itr.base()); }
    }
}

void FaPacketDispatcher::removeSubscriber(PacketQueue_Interface* subscriberToRemove, const EnabledMeasurements& headerToUse) noexcept
{
    for (auto itr = _subscribers.rbegin(); itr != _subscribers.rend(); ++itr)
    {
        auto& subscriber = *itr;
        if (subscriberToRemove == subscriber.queueToPush)
        {
            if (headerToUse == subscriber.headerFilter) { _subscribers.erase(itr.base()); }
        }
    }
}

bool FaPacketDispatcher::_tryPushToCompositeDataQueue(const ByteBuffer& byteBuffer, const size_t syncByteIndex,
                                                      const FaPacketProtocol::Metadata& packetDetails) noexcept
{
    VN_PROFILER_TIME_CURRENT_SCOPE();
    if (!anyDataIsEnabled(packetDetails.header.toMeasurementHeader(), _enabledMeasurements)) { return false; }
    auto compositeData = FaPacketProtocol::parsePacket(byteBuffer, syncByteIndex, packetDetails, _enabledMeasurements);
    if (!compositeData.has_value()) { return false; }

    // Copy to the output queue
    auto pCompositeData = _compositeDataQueue->put();
    if (!pCompositeData) { return false; }
    *pCompositeData = compositeData.value();  // Todo 477: INvestigate passing pointer into the parser, rather than returning and copying it. Will that
                                              // be more efficient than calling "reset" and assigning values?
    return true;
}

void FaPacketDispatcher::_invokeSubscribers(const ByteBuffer& byteBuffer, const size_t syncByteIndex, const FaPacketProtocol::Metadata& packetDetails) noexcept
{
    VN_PROFILER_TIME_CURRENT_SCOPE();
    for (auto& subscriber : _subscribers)
    {
        const auto filterHeader = subscriber.headerFilter;
        const auto packetHeader = packetDetails.header.toMeasurementHeader();
        bool pushToSub;
        switch (subscriber.filterType)
        {
            case (SubscriberFilterType::AnyMatch):
            {
                pushToSub = anyDataIsEnabled(filterHeader, packetHeader);
                break;
            }
            case (SubscriberFilterType::ExactMatch):
            {
                pushToSub = filterHeader == packetHeader;
                break;
            }
            case (SubscriberFilterType::NotExactMatch):
            {
                pushToSub = filterHeader != packetHeader;
                break;
            }
            default:
                VN_ABORT();
        }
        if (pushToSub) { [[maybe_unused]] const bool failed = _tryPushToSubscriber(byteBuffer, syncByteIndex, packetDetails, subscriber); }
    }
}

bool FaPacketDispatcher::_tryPushToSubscriber(const ByteBuffer& byteBuffer, const size_t syncByteIndex, const FaPacketProtocol::Metadata& packetDetails,
                                              Subscriber& subscriber) noexcept
{
    auto putSlot = subscriber.queueToPush->put();
    if (putSlot)
    {
        putSlot->details.syncByte = PacketDetails::SyncByte::FA;
        putSlot->details.faMetadata = packetDetails;
        byteBuffer.peek_unchecked(putSlot->buffer, packetDetails.length, syncByteIndex);
    }
    else
    {
        // Putting failed
        return true;
    }
    return false;
}

}  // namespace VN
