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

#ifndef TEMPLATELIBRARY_STRING_HPP
#define TEMPLATELIBRARY_STRING_HPP

#include <cerrno>
#include <charconv>
#include <cstddef>
#include <cstdlib>
#include <algorithm>
#include <iterator>
#include <limits>
#include <optional>
#include <cstring>
#include <cstdint>
#include <string>
#include <type_traits>
#include <TemplateLibrary/Vector.hpp>
#include "Debug.hpp"

namespace VN
{

template <size_t Capacity>
class String
{
    static_assert(Capacity < size_t(-1));

public:
    String() = default;

    String(const char* inputString, size_t length = Capacity)
    {
        const size_t copyLength = std::min(Capacity, length);
        strncpy(str.data(), inputString, copyLength);
        str[copyLength] = '\0';  // Explicity set null termination for compiler
    }

    String(const char* begin, const char* end) : String(begin, std::distance(begin, end)) {}

    String(const std::string& inputString) : String(inputString.c_str(), inputString.size() + 1) {}

    String(const char& inputChar) { str[0] = inputChar; }

    template <size_t CapacityR>
    String(const String<CapacityR>& other)
    {
        const size_t copyLength = std::min(Capacity, other.length());
        memcpy(str.data(), other.begin(), copyLength);  // Using memcpy because gcc was pushing a truncation warning
        str[copyLength] = '\0';                         // Explicity set null termination
    }

    const char* data() const noexcept { return str.data(); }
    char* data() noexcept { return str.data(); }

    auto c_str() const noexcept { return str.data(); }

    static constexpr size_t capacity() noexcept { return Capacity; }

    size_t numAvailable() const noexcept { return Capacity - length(); }

    char& at(size_t index) noexcept
    {
        VN_ASSERT(index <= length());
        return str.at(index);
    }

    char at(size_t index) const noexcept
    {
        VN_ASSERT(index <= length());
        return str.at(index);
    }

    bool push_back(const char charToPush) noexcept
    {
        if (length() >= Capacity) { return true; }
        str[length()] = charToPush;
        return false;
    }

    void pop_back() noexcept
    {
        if (empty()) { return; }
        str[length() - 1] = '\0';
    }

    char back() noexcept { return empty() ? '\0' : str[length() - 1]; }

    void clear() noexcept { std::fill(str.begin(), str.end(), '\0'); }

    const char* begin() const noexcept { return str.data(); }
    char* begin() noexcept { return str.data(); }

    const char* end() const noexcept { return &str[length()]; }
    char* end() noexcept { return &str[length()]; }

    size_t length() const noexcept { return strnlen(str.data(), Capacity); }

    bool empty() const noexcept { return (str[0] == '\0'); }

    const String<Capacity> substr(const size_t index, const size_t len) const noexcept
    {
        if (index > Capacity) { return String<Capacity>(); }
        else { return String<Capacity>(&str[index], len); }
    }

    const String<Capacity> substr(const size_t index) const noexcept
    {
        if (index > Capacity) { return String<Capacity>(); }
        else { return String<Capacity>(&str[index]); }
    }

    size_t find(const String<Capacity>& substr, size_t startIndex = 0) const noexcept
    {
        size_t substrLength = substr.length();
        if (substrLength > length()) { return String<Capacity>::npos; }
        if (startIndex > (length() - substrLength)) { return String<Capacity>::npos; }

        for (size_t i = startIndex; i <= (length() - substrLength); i++)
        {
            size_t j = 0;
            for (j = 0; j < substrLength; j++)
            {
                if (str[i + j] != substr[j]) { break; }
            }
            if (j == substrLength) { return i; }
        }
        return String<Capacity>::npos;
    }

    size_t find(const char& substr, size_t startIndex = 0) const noexcept
    {
        if (startIndex > length()) { return String<Capacity>::npos; }

        for (size_t j = startIndex; j < length(); j++)
        {
            if (str[j] == substr) return j;
        }
        return String<Capacity>::npos;
    }

    size_t rfind(const char* substr, const size_t startIndex, const size_t substrLength) const noexcept
    {
        for (size_t i = (std::min(length() - substrLength, startIndex)); i != std::numeric_limits<size_t>::max(); i--)
        {
            size_t j = 0;
            for (j = 0; j < substrLength; j++)
            {
                if (str[i + j] != substr[j]) { break; }
            }
            if (j == substrLength) { return (i == std::numeric_limits<size_t>::max()) ? 0 : i; }
            if (i == 0) { return String<Capacity>::npos; }
        }
        return String<Capacity>::npos;
    }

    size_t rfind(const String<Capacity>& substr, size_t startIndex = Capacity) const noexcept { return rfind(substr.c_str(), startIndex, substr.length()); }

    std::string to_string() const noexcept { return std::string(str.data(), length()); }

    operator std::string() const noexcept { return to_string(); }

    char& operator[](size_t index) { return str[index]; }

    const char& operator[](size_t index) const noexcept { return str[index]; }

    inline static const size_t npos = std::numeric_limits<size_t>::max();

private:
    std::array<char, Capacity + 1> str = {0};
};

// ###################
// Operators
// ###################

template <size_t lhsMaxSize>
std::ostream& operator<<(std::ostream& lhs, const String<lhsMaxSize>& rhs) noexcept
{
    lhs << rhs.c_str();
    return lhs;
}

// ******************
// Addition operators
// ******************

// string + const char
template <size_t Capacity>
String<Capacity> operator+(String<Capacity> lhs, const char rhs) noexcept
{
    lhs.push_back(rhs);
    return lhs;
}

// Inversion of above
template <size_t Capacity>
String<Capacity> operator+(const char lhs, const String<Capacity>& rhs) noexcept
{
    return String<Capacity>(lhs) + rhs;
}

// string + array
template <size_t CapacityL, size_t CapacityR>
String<CapacityL> operator+(String<CapacityL> lhs, const char (&rhs)[CapacityR]) noexcept
{
    const size_t rhsLen = strnlen(rhs, CapacityR);
    const size_t rem = CapacityL - lhs.length();
    const size_t num = (rem >= rhsLen) ? rhsLen : rem;
    std::copy_n(rhs, num, lhs.end());
    return lhs;
}

// Inversion of above
template <size_t CapacityL, size_t CapacityR>
String<CapacityR> operator+(const char (&lhs)[CapacityL], const String<CapacityR>& rhs) noexcept
{
    return String<CapacityR>(lhs) + rhs;
}

// string + std::string
template <size_t Capacity>
String<Capacity> operator+(String<Capacity> lhs, const std::string& rhs) noexcept
{
    return lhs + String<Capacity>(rhs);
}

// Inversion of above
template <size_t Capacity>
String<Capacity> operator+(const std::string& lhs, const String<Capacity>& rhs) noexcept
{
    return String<Capacity>(lhs) + rhs;
}

// string + string
template <size_t CapacityL, size_t CapacityR>
String<CapacityL> operator+(String<CapacityL> lhs, const String<CapacityR>& rhs) noexcept
{
    const size_t rhsLen = rhs.length();
    const size_t rem = CapacityL - lhs.length();
    const size_t num = (rem >= rhsLen) ? rhsLen : rem;
    std::copy_n(rhs.begin(), num, lhs.end());
    return lhs;
}

// ******************
// Equality operators
// ******************

template <size_t CapacityL, size_t CapacityR>
bool operator!=(const String<CapacityL>& lhs, const char (&rhs)[CapacityR]) noexcept
{
    return !std::equal(lhs.begin(), lhs.end(), rhs, rhs + strnlen(rhs, CapacityR));
}

template <size_t CapacityL, size_t CapacityR>
inline bool operator==(const String<CapacityL>& lhs, const String<CapacityR>& rhs) noexcept
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <size_t CapacityL, size_t CapacityR>
bool operator==(const char (&lhs)[CapacityR], const String<CapacityL>& rhs) noexcept
{
    return std::equal(rhs.begin(), rhs.end(), lhs, lhs + strnlen(lhs, CapacityR));
}

template <size_t CapacityL, size_t CapacityR>
bool operator==(const String<CapacityL>& lhs, const char (&rhs)[CapacityR]) noexcept
{
    return std::equal(lhs.begin(), lhs.end(), rhs, rhs + strnlen(rhs, CapacityR));
}

template <size_t Capacity>
bool operator==(const std::string& lhs, const String<Capacity>& rhs) noexcept
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <size_t Capacity>
bool operator==(const String<Capacity>& lhs, const std::string& rhs) noexcept
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

namespace StringUtils
{
// ###################
// Numeric Conversions
// ###################

template <class T>
std::optional<T> fromString(const char* begin, const char* end)
{
    static_assert(std::is_arithmetic<T>::value, "Template parameter T must be a numeric type");
    T numReturn;
    if (*begin == '+') { begin++; }
    auto [ptr, ec] = std::from_chars(begin, end, numReturn);
    if (ptr != end || ec != std::errc{}) { return std::nullopt; }
    return std::make_optional(numReturn);
}

#if (defined(__clang__) && __clang_major < 16) || defined(_MSC_VER) || (defined(__GNUC__) && __GNUC__ < 11)
template <>
inline std::optional<double> fromString(const char* begin, const char* end)
{
    char* endPtr;
    errno = 0;
    const double numReturn = strtod(begin, &endPtr);
    if (endPtr != end || errno == ERANGE) { return std::nullopt; }
    return std::make_optional(numReturn);
}

template <>
inline std::optional<float> fromString(const char* begin, const char* end)
{
    char* endPtr;
    errno = 0;
    const float numReturn = strtof(begin, &endPtr);
    if (endPtr != end || errno == ERANGE) { return std::nullopt; }
    return std::make_optional(numReturn);
}
#endif

template <class T>
std::optional<T> fromStringHex(const char* begin, const char* end)
{
    static_assert(std::is_unsigned<T>::value, "Template parameter must be an unsigned integer type");

    T numReturn;
    if ((*(begin + 1) == 'x') || (*(begin + 1) == 'X')) { begin += 2; }
    auto [ptr, ec] = std::from_chars(begin, end, numReturn, 16);
    if (ptr != end || ec != std::errc{}) { return std::nullopt; }
    return std::make_optional(numReturn);
}

// ############
// Manipulation
// ############

constexpr std::size_t stringSplitMaximumSize = 42;

template <size_t Capacity>
bool startsWith(const String<Capacity>& strIn, const String<Capacity>& pattern) noexcept
{
    return strIn.rfind(pattern, 0) == 0;
}

template <size_t Capacity>
bool startsWith(const String<Capacity>& strIn, const char* pattern) noexcept
{
    return strIn.rfind(pattern, 0, std::strlen(pattern)) == 0;
}

template <size_t Capacity>
String<Capacity> extractAfter(const String<Capacity>& strIn, const char delimiter) noexcept
{
    const size_t idx = strIn.find(delimiter);
    if (idx == String<Capacity>::npos) { return ""; }
    return strIn.substr(idx + 1);
}

template <size_t Capacity>
String<Capacity> extractAfter(const String<Capacity>& strIn, const String<Capacity>& delimiter) noexcept
{
    size_t idx = strIn.find(delimiter);
    if (idx == String<Capacity>::npos) { return ""; }
    idx += delimiter.length() - 1;
    return strIn.substr(idx + 1);
}

template <size_t Capacity>
String<Capacity> extractBetween(const String<Capacity>& strIn, const String<Capacity>& start, const String<Capacity>& end) noexcept
{
    size_t startIndex = strIn.find(start);
    if (startIndex == String<Capacity>::npos) { return ""; }
    startIndex += start.length();
    const size_t endIndex = strIn.find(end, startIndex);
    if (endIndex == String<Capacity>::npos) { return ""; }
    return strIn.substr(startIndex, endIndex - startIndex);
}

template <size_t Capacity>
String<Capacity> extractBetween(const String<Capacity>& strIn, const char start, const char end) noexcept
{
    size_t startIndex = strIn.find(start) + 1;
    if (startIndex == String<Capacity>::npos) { return ""; }
    const size_t endIndex = strIn.find(end, startIndex);
    if (endIndex == String<Capacity>::npos) { return ""; }
    return strIn.substr(startIndex, endIndex - startIndex);
}

template <size_t Capacity>
Vector<String<Capacity>, stringSplitMaximumSize> split(const String<Capacity>& strIn, const char delimiter) noexcept
{
    Vector<String<Capacity>, stringSplitMaximumSize> splitStr;
    String<Capacity> buildingStr;
    for (const auto currChar : strIn)
    {
        if (currChar == delimiter)
        {
            splitStr.push_back(buildingStr);
            buildingStr.clear();
        }
        else { buildingStr = buildingStr + currChar; }
    }
    splitStr.push_back(buildingStr);
    return splitStr;
}

}  // namespace StringUtils
}  // namespace VN

template <size_t Capacity>
struct std::hash<VN::String<Capacity>>
{
    size_t operator()(const VN::String<Capacity>& str) const noexcept
    {
        size_t hash = 0;
        for (size_t i = 0; i < str.length(); ++i) { hash ^= static_cast<size_t>(str[i]) + 0x9e3779b9 + (hash << 6) + (hash >> 2); }
        return hash;
    }
};

#endif  // TEMPLATELIBRARY_STRING_HPP
