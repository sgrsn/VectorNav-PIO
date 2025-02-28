// The MIT License (MIT)
//
// Copyright (c) 2024 Hidaka SATO
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

#ifndef HAL_TIMER_TEENSY_HPP
#define HAL_TIMER_TEENSY_HPP

#include <Arduino.h>            // micros() を使用するため
#include "HAL/Timer_Base.hpp"   // Microseconds, Nanoseconds などの型定義

namespace VN
{

using time_point = std::chrono::steady_clock::time_point;
inline time_point now() { return std::chrono::steady_clock::now(); }

/**
 * @brief Arduino 向けタイマー実装
 *
 * - PC 版では std::chrono::steady_clock を使用していましたが、
 *   Arduino 環境では代わりに micros() を用いて実装しています。
 * - タイマー開始時のマイクロ秒カウンタを `_timeStartedUs` に保持し、
 *   `hasTimedOut()` で経過時間を比較します。
 * - `timeElapsed()` は計測したマイクロ秒をナノ秒へ変換して返します。
 */
class Timer : public Timer_Base
{
public:
    Timer() = default;
    explicit Timer(Microseconds timerLength) 
        : _timerLength(timerLength) 
    {
    }

    /**
     * @brief タイマーのタイムアウト時間を設定
     * @param timerLength タイムアウトまでの時間（マイクロ秒単位）
     */
    void setTimerLength(Microseconds timerLength) noexcept override final
    {
        _timerLength = timerLength;
        _hasStarted = false;
    }

    /**
     * @brief タイマーを開始
     *
     * 内部的に micros() の値を保持し、`hasTimedOut()` での判定に利用します。
     */
    void start() noexcept override final
    {
        _timeStartedUs = micros();
        _hasStarted = true;
    }

    /**
     * @brief タイムアウトしたかどうかを判定
     * @return true ならタイムアウト、false ならまだ時間内または未開始
     */
    bool hasTimedOut() const noexcept override final
    {
        if (!_hasStarted)
        {
            return false;
        }

        // micros() は 32bit なので約 70分毎にオーバーフローする点に注意
        unsigned long elapsed = micros() - _timeStartedUs;
        // _timerLength.count() は std::chrono::microseconds の値 (64bit) だが、
        // Teensy では 32bit にキャストして比較（通常は問題なし）
        return elapsed >= static_cast<unsigned long>(_timerLength.count());
    }

    /**
     * @brief タイマー開始からの経過時間をナノ秒単位で取得
     * @note プロファイリング用途などで使用する想定（SDK 内部）
     */
    Nanoseconds timeElapsed() const noexcept
    {
        if (!_hasStarted)
        {
            return Nanoseconds(0);
        }

        // 現在のマイクロ秒と開始時の差分を取得
        unsigned long elapsedUs = micros() - _timeStartedUs;

        // マイクロ秒 => ナノ秒 変換 (x 1000)
        // std::chrono::nanoseconds へ変換
        return Nanoseconds(static_cast<uint64_t>(elapsedUs) * 1000ULL);
    }

private:
    // タイマー開始時点の micros() 値
    unsigned long _timeStartedUs = 0;

    // タイムアウト時間（PC 版では std::chrono::microseconds）
    Microseconds _timerLength{0};

    // タイマーが start() されたかどうか
    bool _hasStarted = false;
};

}  // namespace VN

#endif  // HAL_TIMER_TEENSY_HPP
