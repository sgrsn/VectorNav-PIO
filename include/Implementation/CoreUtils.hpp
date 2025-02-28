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

#ifndef CORE_COREUTILS_HPP
#define CORE_COREUTILS_HPP

#include <stdint.h>

namespace VN
{

inline void _calculateCheckSum(uint8_t* checksum, uint8_t byte) noexcept { *checksum ^= byte; }

inline uint8_t CalculateCheckSum(uint8_t* buffer, uint64_t bufferSize) noexcept
{
    uint8_t checksum = 0;
    for (uint64_t i = 0; i < bufferSize; i++) { _calculateCheckSum(&checksum, buffer[i]); }
    return checksum;
}

inline void _calculateCRC(uint16_t* crc, uint8_t byte) noexcept
{
    *crc = static_cast<uint16_t>((*crc >> 8) | (*crc << 8));
    *crc ^= byte;
    *crc ^= ((*crc & 0xFF) >> 4);
    *crc ^= ((*crc << 8) << 4);
    *crc ^= (((*crc & 0xFF) << 4) << 1);
}

inline uint16_t CalculateCRC(uint8_t* buffer, size_t bufferSize) noexcept
{
    uint16_t crc = 0;
    for (size_t i = 0; i < bufferSize; i++) { _calculateCRC(&crc, buffer[i]); }
    return crc;
}

}  // namespace VN
#endif  // CORE_COREUTILS_HPP
