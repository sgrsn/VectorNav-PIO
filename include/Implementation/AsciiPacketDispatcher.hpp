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

#ifndef IMPLEMENTATION_ASCIIPACKETDISPATCHER_HPP
#define IMPLEMENTATION_ASCIIPACKETDISPATCHER_HPP

#include <cstdint>
#include <functional>
#include <optional>

#include "Implementation/PacketDispatcher.hpp"
#include "Implementation/MeasurementDatatypes.hpp"
#include "Implementation/CommandProcessor.hpp"
#include "Implementation/AsciiPacketProtocol.hpp"
#include "Implementation/QueueDefinitions.hpp"
#include "Config.hpp"

namespace VN
{
class AsciiPacketDispatcher : public PacketDispatcher
{
public:
    AsciiPacketDispatcher(MeasurementQueue* measurementQueue, EnabledMeasurements enabledMeasurements, CommandProcessor* commandProcessor)
        : PacketDispatcher{{'$'}}, _compositeDataQueue(measurementQueue), _enabledMeasurements(enabledMeasurements), _commandProcessor(commandProcessor)
    {
    }

    PacketDispatcher::FindPacketRetVal findPacket(const ByteBuffer& byteBuffer, const size_t syncByteIndex) noexcept override;

    void dispatchPacket(const ByteBuffer& byteBuffer, const size_t syncByteIndex) noexcept override;

    enum class SubscriberFilterType
    {
        StartsWith,
        DoesNotStartWith
    };

    bool addSubscriber(PacketQueue_Interface* subscriber, const AsciiHeader& headerToUse, SubscriberFilterType filterType) noexcept;

    void removeSubscriber(PacketQueue_Interface* subscriberToRemove) noexcept;
    void removeSubscriber(PacketQueue_Interface* subscriberToRemove, const AsciiHeader& headerToUse) noexcept;

private:
    MeasurementQueue* _compositeDataQueue;
    [[maybe_unused]] EnabledMeasurements _enabledMeasurements;

    AsciiPacketProtocol::Metadata _latestPacketMetadata;
    CommandProcessor* _commandProcessor;
    struct Subscriber
    {
        PacketQueue_Interface* queueToPush;
        AsciiHeader headerFilter;
        SubscriberFilterType filterType;
    };

    static const auto SUBSCRIBER_CAPACITY = Config::PacketDispatchers::asciiPacketSubscriberCapacity;
    using Subscribers = Vector<Subscriber, SUBSCRIBER_CAPACITY>;
    Subscribers _subscribers;

    bool _tryPushToCompositeDataQueue(const ByteBuffer& byteBuffer, const size_t syncByteIndex, const AsciiPacketProtocol::Metadata& metadata,
                                      AsciiPacketProtocol::AsciiMeasurementHeader measEnum) noexcept;
    void _invokeSubscribers(const ByteBuffer& byteBuffer, const size_t syncByteIndex, const AsciiPacketProtocol::Metadata& metadata) noexcept;
    bool _tryPushToSubscriber(const ByteBuffer& byteBuffer, const size_t syncByteIndex, const AsciiPacketProtocol::Metadata& metadata,
                              Subscriber& subscriber) noexcept;
};
}  // namespace VN

#endif  // IMPLEMENTATION_ASCIIPACKETDISPATCHER_HPP
