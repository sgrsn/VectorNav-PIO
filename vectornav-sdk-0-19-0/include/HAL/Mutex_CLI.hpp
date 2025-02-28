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

#ifndef HAL_MUTEX_CLI_HPP
#define HAL_MUTEX_CLI_HPP

#include "Mutex_Base.hpp"
#include <cstdint>
#if _WIN32
#include <Windows.h>
#elif __linux__
#endif
#include <mutex>

namespace VN
{

#if _WIN32

class Mutex : public Mutex_Base
{
public:
    void lock() override final
    {
        // while (this->interlock == MUTEX_LOCKED || InterlockedCompareExchange(&this->interlock, MUTEX_LOCKED, MUTEX_UNLOCKED) == MUTEX_LOCKED) {};
        _mutex.lock();
    };

    void unlock() override final
    {
        // this->interlock = MUTEX_UNLOCKED;
        _mutex.unlock();
    };

private:
    const static uint8_t MUTEX_LOCKED = 1;
    const static uint8_t MUTEX_UNLOCKED = 0;
    volatile uint64_t interlock = 0;
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

#elif __linux__
static_assert(false);
#else
static_assert(false);
#endif

}  // namespace VN

#endif  // HAL_MUTEX_CLI_HPP
