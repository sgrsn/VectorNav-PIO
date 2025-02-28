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

#ifndef HAL_TIMER_PC_HPP
#define HAL_TIMER_PC_HPP

#include "HAL/Timer_Base.hpp"

namespace VN
{

using time_point = std::chrono::steady_clock::time_point;
inline time_point now() { return std::chrono::steady_clock::now(); }

class Timer : Timer_Base
{
public:
    Timer() {}
    Timer(Microseconds timerLength) : _timerLength(timerLength) {}

    void setTimerLength(Microseconds timerLength) noexcept override final
    {
        _timerLength = timerLength;
        _hasStarted = false;
    }

    void start() noexcept override final
    {
        _timeStarted = Clock::now();
        _hasStarted = true;
    }

    bool hasTimedOut() const noexcept override final
    {
        if (!_hasStarted) { return false; }
        return (Clock::now() - _timeStarted) >= _timerLength;
    }

    Nanoseconds timeElapsed() const noexcept
    {
        // Used only for profiling
        if (!_hasStarted) { return 0ns; }
        return Clock::now() - _timeStarted;
    }

private:
    using Clock = std::chrono::steady_clock;

    Clock::time_point _timeStarted;
    Microseconds _timerLength{0};
    bool _hasStarted = false;
};

}  // namespace VN
#endif  // HAL_TIMER_PC_HPP
