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

#ifndef CORE_VECTOR_HPP
#define CORE_VECTOR_HPP

#include <stdexcept>
#include <stdint.h>
#include <initializer_list>
#include <iterator>
#include <array>
#include <optional>
#include "Debug.hpp"
#if (VN_DEBUG_LEVEL > 0)
#include <iostream>
#endif
namespace VN
{

template <typename T, size_t Capacity>
class Vector
{
    static_assert(Capacity < size_t(-1));

public:
    Vector() = default;

    Vector(std::initializer_list<T> init) noexcept
    {
        // There should be a static VN_ASSERT here, but initilizer list size is not constexpr until c++14
        VN_ASSERT(init.size() <= Capacity);
        for (const auto& itr : init)
        {
            bool retVal = push_back(itr);
            VN_ASSERT(!retVal);
        }
    }

    template <size_t inputArraySize>
    Vector(const std::array<T, inputArraySize>& arr) noexcept
    {
        static_assert(inputArraySize <= Capacity, "[Error]: Input array exceeds the capacity of the Vector");
        for (const auto& element : arr)
        {
            bool retVal = array.push_back(element);
            VN_ASSERT(!retVal);
        }
    }

    bool push_back(T&& input) noexcept
    {
        if (_size >= Capacity) { return true; }
        else
        {
            array.at(_size++) = std::move(input);
            return false;
        }
    }

    bool push_back(const T& input) noexcept
    {
        if (_size >= Capacity) { return true; }
        else
        {
            array.at(_size++) = input;
            return false;
        }
    }

    bool pop_back() noexcept
    {
        if (_size == 0) { return true; }
        else
        {
            _size -= 1;
            return false;
        }
    }

    constexpr void clear() noexcept { _size = 0; }

#if (VN_DEBUG_LEVEL > 0)
    void print() const noexcept
    {
        const T* i = array.data();
        std::cout << "{";
        for (int j = 0; j < _size; j++)
        {
            if constexpr (std::is_same<T, uint8_t>::value)
            {
                std::cout << "  0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i[j]) << "  ";
            }
            else if constexpr (std::is_same<T, uint16_t>::value) { std::cout << "  0x" << std::hex << std::setfill('0') << std::setw(4) << i[j] << "  "; }
            // else if constexpr (std::is_arithmetic<T>::value) { std::cout << "  " << std::to_string(i[j]) << "  "; }
            else { std::cout << "  " << i[j] << "  "; }
        }
        std::cout << std::dec << "}";  // Reset to decimal format
    }
#endif

    const T& front() const noexcept { return array.at(0); }         // Undefined behavior when empty, as std::vector
    T& front() noexcept { return array.at(0); }                     // Undefined behavior when empty, as std::vector
    const T& back() const noexcept { return array.at(_size - 1); }  // Undefined behavior when empty, as std::vector
    T& back() noexcept { return array.at(_size - 1); }              // Undefined behavior when empty, as std::vector

    using iterator = typename std::array<T, Capacity + 1>::iterator;
    using const_iterator = typename std::array<T, Capacity + 1>::const_iterator;

    iterator begin() noexcept { return array.begin(); }
    const_iterator begin() const noexcept { return array.begin(); }

    iterator end() noexcept { return array.begin() + _size; }
    const_iterator end() const noexcept { return array.begin() + _size; }

    using reverse_iterator = typename std::array<T, Capacity + 1>::reverse_iterator;
    using const_reverse_iterator = typename std::array<T, Capacity + 1>::const_reverse_iterator;

    reverse_iterator rbegin() noexcept { return array.rbegin(); }
    const_reverse_iterator rbegin() const noexcept { return array.rbegin(); }

    reverse_iterator rend() noexcept { return array.rbegin() - _size; }
    const_reverse_iterator rend() const noexcept { return array.rbegin() - _size; }

    const T& at(const size_t i) const noexcept
    {
        if (_size <= i)
        {
            VN_ABORT();  // We don't have an optional return, so we will just fault here. You should be checking the size before using.
        }
        else { return array.at(i); }
    }

    T& at(const size_t i) noexcept
    {
        if (_size <= i)
        {
            VN_ABORT();  // We don't have an optional return, so we will just fault here. You should be checking the size before using.
        }
        else { return array.at(i); }
    }

    bool insert(const_iterator position, const T& value) noexcept
    {
        const size_t index = std::distance<const_iterator>(begin(), position);

        if (index > _size || _size >= Capacity) { return true; }

        for (size_t i = _size; i > index; --i) { std::swap(array[i], array[i - 1]); }

        array[index] = value;
        ++_size;

        return false;
    }

    bool erase(const_iterator position) noexcept
    {
        const size_t index = std::distance<const_iterator>(begin(), position);

        // Invalid index or empty container
        if (index >= _size || _size == 0) { return true; }

        for (size_t i = index; i < _size - 1; ++i) { std::swap(array[i], array[i + 1]); }

        --_size;
        return false;
    }

    // Getters
    bool full() const noexcept { return _size == Capacity; }
    bool empty() const noexcept { return _size == 0; }
    size_t capacity() const noexcept { return Capacity; }
    size_t size() const noexcept { return _size; }
    const T* data() const noexcept { return array.data(); }

    const T& operator[](int idx) const noexcept { return array[idx]; }

    T& operator[](int idx) noexcept { return array[idx]; }

private:
    std::array<T, Capacity + 1> array{};
    size_t _size = 0;
};
template <typename T, size_t Capacity>
bool operator==(const Vector<T, Capacity>& lhs, const Vector<T, Capacity>& rhs)
{
    if (lhs.size() != rhs.size()) { return false; }
    for (size_t i = 0; i < lhs.size(); i++)
    {
        if (lhs.at(i) != rhs.at(i)) { return false; }
    }
    return true;
}

}  // namespace VN

#endif  // CORE_VECTOR_HPP
