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

#ifndef DATAEXPORT_EXPORTERCSVUTILS_HPP
#define DATAEXPORT_EXPORTERCSVUTILS_HPP

#include <string>
#include <stdint.h>
#include "Implementation/FaPacketProtocol.hpp"
#include "Implementation/AsciiPacketProtocol.hpp"

namespace VN
{

enum CsvType : uint8_t
{
    U8,
    U16,
    U32,
    U64,
    UTC,
    FLO,
    DUB,
    SAT,
    RAW,
    UNK,
    NON,
};

using VN::CsvType;

struct CsvTypeInfo
{
    CsvType type;
    uint8_t len;
};

CsvTypeInfo csvTypeLookup(size_t i, size_t j);

template <class T>
int extractToString(FaPacketExtractor& extractor, const size_t numToExtract, char* ptr, const uint16_t remaining) noexcept
{
    int offset = 0;
    uint8_t first_comma_skip = 1;  // Skip first comma of sentense
    for (uint8_t i = 0; i < numToExtract; i++)
    {
        constexpr const char* format = []() constexpr
        {
            if constexpr (std::is_same_v<T, float>) { return ",%f"; }
            else if constexpr (std::is_same_v<T, double>) { return ",%12.8f"; }
            else if constexpr (std::is_same_v<T, uint64_t>) { return ",%llu"; }
            else { return ",%d"; }
        }();
        offset += std::snprintf(ptr + offset, remaining - offset - first_comma_skip, format + first_comma_skip, extractor.extract_unchecked<T>());
        first_comma_skip = 0;
    }
    return offset;
}

int getMeasurementString(FaPacketExtractor& extractor, const CsvTypeInfo& typeInfo, char* ptr, const uint16_t remaining);

const char* getMeasurementString(const AsciiPacketProtocol::AsciiMeasurementHeader& msg);

const char* getMeasurementName(const size_t binaryGroup, const size_t binaryField);

}  // namespace VN

#endif  // DATAEXPORT_EXPORTERCSVUTILS_HPP
