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

#include "Implementation/FbPacketProtocol.hpp"

namespace VN
{
namespace FbPacketProtocol
{
bool _isValidBinaryCrc(const ByteBuffer& buffer, const size_t syncByteIndex, const size_t packetLength) noexcept
{
    uint16_t calculatedCrc = 0;
    // Crc validation does not include sync byte
    for (size_t fromHeadIndex = syncByteIndex + 1; fromHeadIndex < (syncByteIndex + packetLength); ++fromHeadIndex)
    {
        _calculateCRC(&calculatedCrc, buffer.peek_unchecked(fromHeadIndex));
    }
    return calculatedCrc == 0;
}

FbPacketProtocol::FindPacketReturn findPacket(const ByteBuffer& byteBuffer, const size_t syncByteIndex) noexcept
{
    Metadata metadata;
    const size_t headerSize = 5;
    size_t currentFromHeadIndex = syncByteIndex;

    // Get the first byte and verify it is 0xFB
    const uint8_t syncByte = byteBuffer.peek_unchecked(syncByteIndex);
    if (syncByte != 0xFB) { return FindPacketReturn{Validity::Invalid, Metadata{}}; }  // It was a mistake to come here.

    // Verify this is a message type that we support; the only supported message type is 0
    metadata.header.messageType = byteBuffer.peek_unchecked(++currentFromHeadIndex);
    if (metadata.header.messageType != 0) { return FindPacketReturn{Validity::Invalid, Metadata{}}; }

    const size_t numPacketBytesInBuffer = byteBuffer.size() - syncByteIndex;

    const size_t minPossiblePacketLength = 1 + headerSize + 1 + 2;  // Sync byte + FB header (5) + 1 Payload + CRC
    if (numPacketBytesInBuffer < minPossiblePacketLength) { return FindPacketReturn{Validity::Incomplete, Metadata{}}; }

    metadata.header.messageId = byteBuffer.peek_unchecked(++currentFromHeadIndex);

    // Get the number of packets associate with this message and the current packet index
    const uint8_t PacketCount = byteBuffer.peek_unchecked(++currentFromHeadIndex);
    const uint8_t totalNumPackets = (PacketCount & 0xF0) >> 4;
    const uint8_t currentPacketCount = (PacketCount & 0x0F);

    if (currentPacketCount > totalNumPackets) { return FindPacketReturn{Validity::Invalid, Metadata{}}; }
    metadata.header.totalPacketCount = totalNumPackets;
    metadata.header.currentPacketCount = currentPacketCount;

    metadata.header.payloadLength = byteBuffer.peek_unchecked(++currentFromHeadIndex);
    metadata.header.payloadLength += (byteBuffer.peek_unchecked(++currentFromHeadIndex) << 8);

    metadata.length = (1 + headerSize + metadata.header.payloadLength + 2);  // Minimum requirement is 1 sync, 5 header, payload, and 2 CRC
    if (metadata.length > MAX_PACKET_LENGTH) { return FindPacketReturn{Validity::Invalid, Metadata{}}; }

    if (numPacketBytesInBuffer < metadata.length) { return FindPacketReturn{Validity::Incomplete, Metadata{}}; }

    const bool isValidCrc = _isValidBinaryCrc(byteBuffer, syncByteIndex, metadata.length);
    if (!isValidCrc) { return FindPacketReturn{Validity::Invalid, Metadata{}}; }

    return FindPacketReturn{Validity::Valid, metadata};
}
}  // namespace FbPacketProtocol
}  // namespace VN
