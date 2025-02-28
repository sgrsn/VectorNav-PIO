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

#ifndef IMPLEMENTATION_FAPACKETDISPATCHER_HPP
#define IMPLEMENTATION_FAPACKETDISPATCHER_HPP

#include <cstdint>
#include <functional>
#include <optional>
#include <limits>

#include "TemplateLibrary/ByteBuffer.hpp"
#include "TemplateLibrary/Vector.hpp"
#include "Implementation/PacketDispatcher.hpp"
#include "Implementation/FaPacketProtocol.hpp"
#include "Implementation/QueueDefinitions.hpp"
#include "Implementation/BinaryHeader.hpp"
#include "Config.hpp"

namespace VN
{

class FaPacketDispatcher : public PacketDispatcher
{
public:
    FaPacketDispatcher(MeasurementQueue* measurementQueue, EnabledMeasurements enabledMeasurements)
        : PacketDispatcher({0xFA}), _compositeDataQueue(measurementQueue), _enabledMeasurements(enabledMeasurements)
    {
    }

    PacketDispatcher::FindPacketRetVal findPacket(const ByteBuffer& byteBuffer, const size_t syncByteIndex) noexcept override;

    void dispatchPacket(const ByteBuffer& byteBuffer, const size_t syncByteIndex) noexcept override;

    enum class SubscriberFilterType
    {
        ExactMatch,
        AnyMatch,
        NotExactMatch
    };

    bool addSubscriber(PacketQueue_Interface* subscriber, EnabledMeasurements headerToUse, SubscriberFilterType filterType) noexcept;

    void removeSubscriber(PacketQueue_Interface* subscriberToRemove) noexcept;
    void removeSubscriber(PacketQueue_Interface* subscriberToRemove, const EnabledMeasurements& headerToUse) noexcept;

protected:
    struct Subscriber
    {
        PacketQueue_Interface* queueToPush;
        EnabledMeasurements headerFilter;
        SubscriberFilterType filterType;
    };

    static const auto SUBSCRIBER_CAPACITY = Config::PacketDispatchers::faPacketSubscriberCapacity;
    using Subscribers = Vector<Subscriber, SUBSCRIBER_CAPACITY>;
    Subscribers _subscribers;

    MeasurementQueue* _compositeDataQueue;
    EnabledMeasurements _enabledMeasurements;
    FaPacketProtocol::Metadata _latestPacketMetadata;

    bool _tryPushToCompositeDataQueue(const ByteBuffer& byteBuffer, const size_t syncByteIndex, const FaPacketProtocol::Metadata& packetDetails) noexcept;
    void _invokeSubscribers(const ByteBuffer& byteBuffer, const size_t syncByteIndex, const FaPacketProtocol::Metadata& packetDetails) noexcept;
    bool _tryPushToSubscriber(const ByteBuffer& byteBuffer, const size_t syncByteIndex, const FaPacketProtocol::Metadata& packetDetails,
                              Subscriber& subscriber) noexcept;
};

}  // namespace VN

#endif  // IMPLEMENTATION_FAPACKETDISPATCHER_HPP
