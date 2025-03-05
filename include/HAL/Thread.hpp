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

#ifndef HAL_THREAD_HPP
#define HAL_THREAD_HPP

// プラットフォーム検出と適切なスレッド実装の選択
#if defined(TEENSYDUINO)
    // Teensy用の実装
    #include "HAL/Thread_Mbed.hpp"
#elif defined(ESP32)
    // ESP32用の実装
    #include "HAL/Thread_ESP32.hpp"
#elif defined(ARDUINO_ARCH_STM32) && defined(ARDUINO_CMSIS_RTOS_ENABLED)
    // STM32 + CMSIS-RTOS用の実装
    #include "HAL/Thread_STM32.hpp"
#elif defined(THREADING_ENABLE) && (THREADING_ENABLE == false)
    // スレッドを明示的に無効化
    #include "HAL/Thread_Disabled.hpp"
#else
    // その他のプラットフォーム用のフォールバック実装
    #include "HAL/Thread_Disabled.hpp"
#endif

#endif  // HAL_THREAD_HPP
