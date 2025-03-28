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

#ifndef HAL_MUTEX_PC_HPP
#define HAL_MUTEX_PC_HPP

#include <mutex>
#include "HAL/Mutex_Base.hpp"

namespace VN
{
class Mutex : public Mutex_Base
{
public:
    Mutex() {}

    void lock() override final { _mutex.lock(); }
    void unlock() override final { _mutex.unlock(); }

private:
    std::mutex _mutex;
};

class LockGuard : public LockGuard_Base
{
public:
    LockGuard(Mutex& mutex) : _mutex(mutex) { _mutex.lock(); }

    ~LockGuard() { _mutex.unlock(); }

private:
    Mutex& _mutex;
};
}  // namespace VN

#endif  // HAL_MUTEX_PC_HPP
