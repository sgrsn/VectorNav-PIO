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

#ifndef IMPLEMENTATION_FBPACKETPROTOCOL_HPP
#define IMPLEMENTATION_FBPACKETPROTOCOL_HPP

#include <cstdint>

#include "TemplateLibrary/ByteBuffer.hpp"
#include "Implementation/PacketDispatcher.hpp"
#include "Implementation/CoreUtils.hpp"
#include "Interface/CompositeData.hpp"
#include "Config.hpp"

namespace VN
{

struct SplitPacketDetails
{
    uint8_t messageType = 0;
    uint8_t messageId = 0;
    uint8_t totalPacketCount = 0;
    uint8_t currentPacketCount = 0;
    uint16_t payloadLength = 0;

    bool operator==(const SplitPacketDetails& rhs) const
    {
        if (messageType == rhs.messageType && messageId == rhs.messageId && totalPacketCount == rhs.totalPacketCount &&
            currentPacketCount == rhs.currentPacketCount && payloadLength == rhs.payloadLength)
        {
            return true;
        }
        return false;
    }
};

namespace FbPacketProtocol
{

using Validity = PacketDispatcher::FindPacketRetVal::Validity;
constexpr size_t MAX_PACKET_LENGTH = VN::Config::PacketFinders::fbPacketMaxLength;

struct Metadata
{
    SplitPacketDetails header;
    size_t length;
};

struct FindPacketReturn
{
    Validity validity;
    Metadata metadata;
};

FindPacketReturn findPacket(const ByteBuffer& byteBuffer, const size_t syncByteIndex) noexcept;

}  // namespace FbPacketProtocol
}  // namespace VN

#endif  // IMPLEMENTATION_FBPACKETPROTOCOL_HPP
