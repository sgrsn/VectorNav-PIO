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

#ifndef TEMPLATELIBRARY_MATRIX_HPP
#define TEMPLATELIBRARY_MATRIX_HPP

#include <algorithm>
#include <initializer_list>
#include <cmath>
#include <array>

#include "Debug.hpp"

namespace VN
{

template <size_t m, size_t n, typename T>
struct Matrix
{
    Matrix(T val = 0) noexcept { std::fill(_data.begin(), _data.end(), val); }

    Matrix(std::initializer_list<T> init) noexcept
    {
        VN_ASSERT(m * n == init.size());
        std::copy(init.begin(), init.end(), _data.begin());
    }

    Matrix(const std::array<T, m * n>& init) noexcept { _data = init; }

    static Matrix identity()
    {
        static_assert(m == n);
        Matrix<m, m, T> nm;
        for (size_t i = 0; i < m; i++) { nm(i, i) = 1; };
        return nm;
    }

    inline static Matrix null(){return Matrix<m, m, T> {};}
    
    // Compound Assignment Operator Overloads /////////////////////////////////////////////////////
    const T& operator()(size_t row, size_t col) const
    {
        VN_ASSERT(row < m && col < n);
        return _data[col + row * n];
    }

    T& operator()(size_t row, size_t col)
    {
        VN_ASSERT(row < m && col < n);
        return _data[col + row * n];
    }

    const T& operator()(size_t row) const
    {
        VN_ASSERT(row < m * n);
        return _data[row];
    }

    T& operator()(size_t row)
    {
        VN_ASSERT(row < m * n);
        return _data[row];
    }

    T& operator[](size_t idx) { return _data[idx]; }

    const T& operator[](size_t idx) const { return _data[idx]; }

    constexpr size_t rows() const { return m; }

    constexpr size_t cols() const { return n; }

    constexpr T* data() noexcept { return _data.data(); }

    constexpr const T* data() const noexcept { return _data.data(); }

    constexpr size_t size() const noexcept { return _data.size(); }

    using iterator = typename std::array<T, m * n>::iterator;
    using const_iterator = typename std::array<T, m * n>::const_iterator;

    constexpr iterator begin() noexcept { return _data.begin(); }
    constexpr const_iterator begin() const noexcept { return _data.begin(); }
    constexpr iterator end() noexcept { return _data.end(); }
    constexpr const_iterator end() const noexcept { return _data.end(); }

    constexpr const_iterator cbegin() const noexcept { return _data.cbegin(); }
    constexpr const_iterator cend() const noexcept { return _data.cend(); }

private:
    std::array<T, m * n> _data = {0};
};

using Mat2i = Matrix<2, 2, int>;

using Mat3i = Matrix<3, 3, int>;

using Mat2f = Matrix<2, 2, float>;

using Mat3f = Matrix<3, 3, float>;

using Mat4f = Matrix<4, 4, float>;

using Mat2d = Matrix<2, 2, double>;

using Mat3d = Matrix<3, 3, double>;

using Mat4d = Matrix<4, 4, double>;

using Vec2f = Matrix<2, 1, float>;

using Vec3f = Matrix<3, 1, float>;

using Vec4f = Matrix<4, 1, float>;

using Vec2d = Matrix<2, 1, double>;

using Vec3d = Matrix<3, 1, double>;
}  // namespace VN

#endif  // TEMPLATELIBRARY_MATRIX_HPP
