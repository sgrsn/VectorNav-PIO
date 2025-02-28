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

#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <filesystem>
#include <assert.h>
#include <iostream>
#define VN_CURRENT_DEBUG_LEVEL 0
#define VN_USING_LIGHTWEIGHT_DEBUG false

#ifndef VN_PROFILING_ENABLE
#define VN_PROFILING_ENABLE false
#endif

#if (VN_USING_LIGHTWEIGHT_DEBUG)
#include <string>
#include <vector>

static std::vector<std::string> LIGHTWEIGHT_DEBUG_VECTOR;

#define VN_LIGHTWEIGHT_DEBUG_LOG(debug_expression) LIGHTWEIGHT_DEBUG_VECTOR.emplace_back(debug_expression);

#define VN_LIGHTWEIGHT_DEBUG_PRINT()                                               \
    for (const auto& msg : LIGHTWEIGHT_DEBUG_VECTOR) { std::cout << msg << "\n"; } \
    LIGHTWEIGHT_DEBUG_VECTOR.clear();
#endif

#define __FILENAME__ std::filesystem::path(__FILE__).filename().string()

#define VN_ASSERT(assert_expression)                                     \
    if (!(assert_expression))                                            \
    {                                                                    \
        std::cout << "(" << __FILENAME__ << ":" << __LINE__ << ") "      \
                  << "Assertion failed: " << #assert_expression << "\n"; \
        abort();                                                         \
    }

#define VN_ABORT()                                              \
    std::cout << "(" << __FILENAME__ << ":" << __LINE__ << ") " \
              << "Abort called."                                \
              << "\n";                                          \
    abort();

#define FORCE_DEBUG(debug_expression) std::cout << "(" << __FILENAME__ << ":" << __LINE__ << ") " << (debug_expression) << "\n";

#if (VN_CURRENT_DEBUG_LEVEL >= 1)
#define VN_DEBUG_1(debug_expression) FORCE_DEBUG(debug_expression)
#else
#define VN_DEBUG_1(debug_expression)
#endif

#if (VN_CURRENT_DEBUG_LEVEL >= 2)
#define VN_DEBUG_2(debug_expression) FORCE_DEBUG(debug_expression)
#else
#define VN_DEBUG_2(debug_expression)
#endif

#if (VN_PROFILING_ENABLE)
#include "HAL/Timer.hpp"

struct TimeThings
{
    uint64_t timeElapsedNs = 0;
    uint64_t count = 0;
};

#include <map>
#include <unordered_map>
#include <array>

inline std::unordered_map<uint32_t, TimeThings> timers{};
inline std::unordered_map<uint32_t, std::string> timer_names{};

#define VN_PROFILER_ADD_STRING_ONCE(key, value) \
    do {                                        \
        static bool added_##key = false;        \
        if (!added_##key)                       \
        {                                       \
            (timer_names)[key] = value;         \
            added_##key = true;                 \
        }                                       \
    } while (0)

class ScopedStopwatch : public VN::Timer
{
public:
    ScopedStopwatch(uint32_t timerId) : Timer(), _timerId(timerId) { start(); }
    ~ScopedStopwatch()
    {
        timers[_timerId].timeElapsedNs += timeElapsed().count();
        ++timers[_timerId].count;
    }

private:
    uint32_t _timerId;
};

constexpr uint32_t fnv1a(const char* str, uint32_t hash = 2166136261u)
{
    return (*str == '\0') ? hash : fnv1a(str + 1, (hash ^ static_cast<uint32_t>(*str)) * 16777619u);
}

#define VN_PROFILER_S1(x) #x
#define VN_PROFILER_S2(x) VN_PROFILER_S1(x)
#define VN_PROFILER_FILE_AND_LINE __FILE__ "-" VN_PROFILER_S2(__LINE__)

#define VN_PROFILER_TIME_CURRENT_SCOPE()                               \
    constexpr uint32_t hashed_id = fnv1a(VN_PROFILER_FILE_AND_LINE);   \
    VN_PROFILER_ADD_STRING_ONCE(hashed_id, VN_PROFILER_FILE_AND_LINE); \
    ScopedStopwatch SCOPED_STOPWATCH_##hashed_id(hashed_id);

#define VN_PROFILER_PRINT_TIMERS()                                                                                                                 \
    std::map<std::string, TimeThings> ordered_timers{};                                                                                            \
    for (const auto& timer : timers) { ordered_timers[timer_names[timer.first]] = timer.second; }                                                  \
    size_t idx = 0;                                                                                                                                \
    for (const auto& timer : ordered_timers)                                                                                                       \
    {                                                                                                                                              \
        std::cout << std::to_string(idx++) << ": " << timer.first << std::endl;                                                                    \
        std::cout << "    Total time (ms): " << std::to_string(float(timer.second.timeElapsedNs) * 1e-6) << std::endl;                             \
        std::cout << "    Hit count      : " << std::to_string(timer.second.count) << std::endl;                                                   \
        std::cout << "    Avg time   (us): " << std::to_string(float(timer.second.timeElapsedNs) / float(timer.second.count) * 1e-3) << std::endl; \
    }
#else
#define VN_PROFILER_TIME_CURRENT_SCOPE()
#define VN_PROFILER_PRINT_TIMERS()
#endif
#endif  // DEBUG_HPP
