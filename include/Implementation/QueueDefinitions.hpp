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

#ifndef IMPLEMENTATION_QUEUEDEFINITIONS_HPP
#define IMPLEMENTATION_QUEUEDEFINITIONS_HPP

#include <memory>

#include "TemplateLibrary/DirectAccessQueue.hpp"
#include "Implementation/Packet.hpp"
#include "Interface/CompositeData.hpp"
#include "Config.hpp"

namespace VN
{
using MeasurementQueue = DirectAccessQueue<CompositeData, Config::PacketDispatchers::compositeDataQueueCapacity>;

using PacketQueue_Interface = DirectAccessQueue_Interface<Packet>;

template <uint16_t Capacity>
using PacketQueue = DirectAccessQueue<Packet, Capacity>;
}  // namespace VN

#endif  // IMPLEMENTATION_QUEUEDEFINITIONS_HPP
