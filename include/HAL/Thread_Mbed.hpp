// The MIT License (MIT)
//
// Copyright (c) Hidaka SATO
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

#ifndef HAL_THREAD_TEENSY_HPP
#define HAL_THREAD_TEENSY_HPP

#include <Arduino.h>
#include <TeensyThreads.h>     // TeensyThreads ライブラリを使用
#include <functional>
#include <utility>
#include "HAL/Thread_Base.hpp"

namespace VN
{

/**
 * @brief Teensy 4.1 + TeensyThreads を使ったスレッドクラス
 *
 * PC版の std::thread ライクなインターフェースを模倣しています。
 * - `join()` : スレッド終了まで待機
 * - `detach()` : デタッチ (TeensyThreads の標準APIには該当機能がないため実装はNo-op)
 * - `joinable()` : スレッドが現在アクティブかどうか
 *
 */
class Thread : public Thread_Base
{
public:
    // スレッドのコピーは禁止
    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;

    /**
     * @brief デフォルトコンストラクタ (スレッドをまだ作成しない)
     */
    Thread() = default;

    /**
     * @brief コンストラクタ：関数オブジェクト + 引数を受け取り、スレッドを起動
     *
     * std::thread と同様の書き方でスレッドを生成できます。
     * 例: `Thread t([]{ ... });`
     */
    template <typename Callable, typename... Args>
    explicit Thread(Callable&& f, Args&&... args)
    {
        // 呼び出す関数をバインドして保存
        _func = std::bind(std::forward<Callable>(f), std::forward<Args>(args)...);

        // TeensyThreads でスレッド生成
        // 第1引数は関数ポインタ (staticメンバ)、
        // 第2引数には this ポインタを渡して、スレッド側で実際のコールバックを呼ぶ
        _threadID = threads.addThread(Thread::_runTrampoline, this);
    }

    /**
     * @brief デストラクタ
     *
     * PC 版の実装では joinable() なら join() しますが、
     * こちらも同じ挙動をとっています。
     * スレッドがまだ生きていれば終了を待機するため、注意してください。
     */
    ~Thread() override
    {
        if (joinable())
        {
            join();
        }
    }

    /**
     * @brief スレッド終了を待機
     */
    void join() override final
    {
        if (joinable())
        {
            threads.wait(_threadID);  // スレッド終了を待機
        }
    }

    /**
     * @brief スレッドのデタッチ
     *
     * TeensyThreads における detach 相当の機能は特にありません。
     * ここでは「何もしない」No-op 実装としています。
     */
    void detach() override final
    {
        // TeensyThreads は detach 概念がないため、何もしない
    }

    /**
     * @brief スレッドがまだ実行中かどうか
     */
    bool joinable() const override final
    {
        return threads.getState(_threadID) == Threads::RUNNING;
    }

private:
    // スレッド実行時に呼び出される静的関数
    static void _runTrampoline(void* arg)
    {
        auto self = static_cast<Thread*>(arg);
        if (self && self->_func)
        {
            self->_func();
        }
        // スレッド終了時は自動的に Threads から削除される
    }

    // TeensyThreads でスレッドを一意に識別する ID
    int _threadID = -1;

    // ユーザが指定した関数を保持する
    std::function<void()> _func;
};


/**
 * @brief 現在実行中のスレッドを操作するための名前空間
 */
namespace thisThread
{
    /**
     * @brief 指定したマイクロ秒だけスリープ (ブロック)
     *
     * PC版の `std::this_thread::sleep_for(...)` と同様の呼び出しを模倣。
     * - TeensyThreads 導入下では `Threads.delay()` を使う選択もありますが、
     *   ここでは単純化のために Arduino 標準の `delay()` / `delayMicroseconds()` を使用。
     */
    inline void sleepFor(const Microseconds sleepDuration) noexcept
    {
        // マイクロ秒値をミリ秒 + 残りマイクロ秒に分割
        const uint64_t totalUs = static_cast<uint64_t>(sleepDuration.count());
        const uint32_t ms = static_cast<uint32_t>(totalUs / 1000ULL);
        const uint32_t us = static_cast<uint32_t>(totalUs % 1000ULL);

        if (ms > 0)
        {
            delay(ms);
        }
        if (us > 0)
        {
            delayMicroseconds(us);
        }
    }
}  // namespace thisThread

}  // namespace VN

#endif  // HAL_THREAD_TEENSY_HPP
