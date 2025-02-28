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

#ifndef IMPLEMENTATION_PACKET_HPP
#define IMPLEMENTATION_PACKET_HPP

#include <array>
#include <cstdint>

#include "AsciiPacketProtocol.hpp"
#include "FaPacketProtocol.hpp"

namespace VN
{

struct PacketDetails
{
    enum class SyncByte : uint8_t
    {
        Ascii = '$',
        FA = 0xFA,
        None = 0,
    } syncByte;

    union
    {
        AsciiPacketProtocol::Metadata asciiMetadata;
        FaPacketProtocol::Metadata faMetadata;
    };
    PacketDetails() : syncByte(SyncByte::None), faMetadata() {}
};

struct Packet
{
    Packet(size_t length) : buffer(new uint8_t[length]), size(length) {}

    template <size_t Capacity>
    Packet(std::array<uint8_t, Capacity>& externalBuffer) : buffer(externalBuffer.data()), size(Capacity), _autoAllocated(false)
    {
    }
    ~Packet()
    {
        if (_autoAllocated) { delete[] buffer; }
    }

    Packet(const Packet&) = delete;
    Packet& operator=(const Packet&) = delete;
    Packet(Packet&&) = delete;
    Packet& operator=(Packet&&) = delete;

    PacketDetails details{};
    uint8_t* buffer;
    size_t size = 0;

private:
    const bool _autoAllocated = true;
};

}  // namespace VN

#endif  // IMPLEMENTATION_PACKET_HPP
