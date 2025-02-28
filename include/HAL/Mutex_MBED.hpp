// The MIT License (MIT)
//
// VectorNav SDK (v0.18.0)
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

#ifndef HAL_MUTEX_TEENSY_HPP
#define HAL_MUTEX_TEENSY_HPP

#include <Arduino.h>         // yield() などを利用
#include <atomic>            // __atomic_* 系の組み込みを使う場合
#include "HAL/Mutex_Base.hpp"

namespace VN
{

/**
 * @brief Teensy 4.1 (Arduino) 向け Mutex 実装
 *
 * - PC 向けの std::mutex が使用できない環境でも動作可能にするため、
 *   アトミック変数を用いたスピンロックを実装例として示しています。
 * - クリティカルセクションが長くなる場合、CPU がスピン待ち状態となるため、
 *   頻繁に lock するようなコードでは応答性に注意してください。
 * - ロックの獲得を待機中に `yield()` を呼び出し、他タスク（Loop）に
 *   CPU を譲ることで、ブロッキングを多少和らげています。
 */
class Mutex : public Mutex_Base
{
public:
    Mutex() : _locked(false) {}

    /**
     * @brief Mutex をロックする
     *
     * ロックが解放されるまでスピンし、取得できない間は yield() を呼んでいます。
     */
    void lock() override final
    {
        // スピンロック: 他が unlock するまで待ち
        while (__atomic_test_and_set(&_locked, __ATOMIC_ACQUIRE))
        {
            // 他タスクへ CPU を譲る
            yield();
        }
    }

    /**
     * @brief Mutex をアンロックする
     */
    void unlock() override final
    {
        __atomic_clear(&_locked, __ATOMIC_RELEASE);
    }

private:
    // ロック状態を表すフラグ (true: ロック中 / false: ロック解除)
    volatile bool _locked;
};


/**
 * @brief LockGuard はスコープライフタイムで自動的にロックを解放するための RAII クラス
 *
 * ```cpp
 * void someFunction() {
 *   static VN::Mutex myMutex;
 *   {
 *       VN::LockGuard lock(myMutex);  // ここでロック取得
 *       // ... クリティカルセクション ...
 *   } // スコープを抜けると自動で unlock()
 * }
 * ```
 */
class LockGuard : public LockGuard_Base
{
public:
    LockGuard(Mutex& mutex)
        : _mutex(mutex)
    {
        _mutex.lock();
    }

    ~LockGuard()
    {
        _mutex.unlock();
    }

private:
    Mutex& _mutex;
};

}  // namespace VN

#endif // HAL_MUTEX_TEENSY_HPP
