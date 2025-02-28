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

#ifndef IMPLEMENTATION_BINARYHEADER_HPP
#define IMPLEMENTATION_BINARYHEADER_HPP

#include <cstddef>
#include <cstdint>
#include "Implementation/MeasurementDatatypes.hpp"
#include "TemplateLibrary/Vector.hpp"
#include "TemplateLibrary/String.hpp"  // Only used for AsciiHeader definition
#include <cstdio>                      // snprintf

namespace VN
{

/// @brief A BinaryHeader holds a binary measurement's header as it comes off the sensor, but parsed into uint16_t and uint8_t.
class BinaryHeader
{
public:
    BinaryHeader() {};

    BinaryHeader(Vector<uint8_t, binaryGroupMaxSize> outputGroupsIn, Vector<uint16_t, binaryTypeMaxSize> outputTypesIn)
        : outputGroups(outputGroupsIn), outputTypes(outputTypesIn) {};

    EnabledMeasurements toMeasurementHeader() const noexcept;

    uint8_t size() const noexcept { return static_cast<uint8_t>(outputGroups.size() + outputTypes.size() * 2); };

    bool operator==(const BinaryHeader& other) const noexcept { return outputGroups == other.outputGroups && outputTypes == other.outputTypes; }
    Vector<uint8_t, binaryGroupMaxSize> outputGroups;
    Vector<uint16_t, binaryTypeMaxSize> outputTypes;

    bool contains(const size_t group, const uint32_t field) const
    {
        const uint32_t combinedGroups = (outputGroups[2] << 16) | ((outputGroups[1] & 0x7F) << 8) | (outputGroups[0] & 0x7F);
        if (!(combinedGroups & group)) { return false; }

        const uint16_t extensionBits = (combinedGroups & (0x8080));

        uint8_t numGroups{};
        if (extensionBits == 0) { numGroups = 1; }
        else if (extensionBits == 0x80) { numGroups = 2; }
        else { numGroups = 3; }

        size_t offset = 0;
        for (size_t i = 0; i < numGroups * 8LU - 1; i++)
        {
            if ((group & (1 << i))) break;

            if ((combinedGroups >> i) & 1) { offset += (outputTypes[offset] & 0x8000) ? 2 : 1; }
        }

        uint32_t combinedField = (outputTypes[offset] & 0x8000) ? (outputTypes[offset + 1] << 16) | (outputTypes[offset] & 0x7FFF) : outputTypes[offset];

        return combinedField & field;
    }

    bool isOutputGroupEnabled(const uint32_t group) const
    {
        const uint32_t enabledGroups = (outputGroups[2] << 16) | ((outputGroups[1] & 0x7F) << 8) | (outputGroups[0] & 0x7F);
        return enabledGroups & group;
    }

private:
    unsigned int _countSetBits(uint8_t n);
};

class BinaryHeaderIterator
{
public:
    BinaryHeaderIterator(const BinaryHeader& header)
        : _header(header), _combinedGroups(((header.outputGroups[2] << 16) | (header.outputGroups[1] << 8) | header.outputGroups[0]))
    {
    }

    bool next()
    {
        for (uint8_t i = _gOffset; i < static_cast<uint8_t>((_numExtGroup + 1) * 8 - 1); i++)
        {
            if (!(_combinedGroups & (0x01 << i))) { continue; }

            const uint16_t field = _header.outputTypes[_offset];

            for (uint8_t j = _fOffset; j < 15; j++)
            {
                if (!(field & (0x01 << j))) { continue; }
                _fOffset = j + 1;
                _gOffset = i;
                _data.group = static_cast<uint8_t>(i);
                _data.field = static_cast<uint8_t>(16 * _numExtField + j);
                return true;
            }

            if (field & 0x8000)
            {
                _offset++;
                _numExtField++;
                _fOffset = 0;
                _gOffset = i;
                return next();
            }
            else
            {
                _offset++;
                _numExtField = 0;
                _fOffset = 0;
            }
        }

        if (_combinedGroups & (0x01 << ((_numExtGroup + 1) * 8 - 1)))
        {
            _gOffset = (_numExtGroup + 1) * 8;
            _numExtGroup++;
            return next();
        }
        return false;
    }

    uint8_t group() const { return _data.group; }

    uint8_t field() const { return _data.field; }

    struct Data
    {
        uint8_t group;
        uint8_t field;
    };

private:
    Data _data;
    uint8_t _numExtGroup = 0;
    uint8_t _numExtField = 0;
    uint8_t _offset = 0;
    uint8_t _gOffset = 0;
    uint8_t _fOffset = 0;
    const BinaryHeader& _header;
    const uint32_t _combinedGroups;
};

template <size_t OutputCapacity>
String<OutputCapacity> binaryHeaderToString(const BinaryHeader& binaryHeader) noexcept
{
    String<OutputCapacity> retVal;
    for (const auto groupByte : binaryHeader.outputGroups) { std::snprintf(retVal.end(), retVal.numAvailable(), ",%02X", groupByte); }
    for (const auto typeWord : binaryHeader.outputTypes) { std::snprintf(retVal.end(), retVal.numAvailable(), ",%04X", typeWord); }
    return retVal;
}

}  // namespace VN

#endif  // IMPLEMENTATION_BINARYHEADER_HPP
