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

#ifndef HAL_THREAD_DISABLED_HPP
#define HAL_THREAD_DISABLED_HPP

#include <Arduino.h>
#include <functional>
#include <utility>
#include "HAL/Thread_Base.hpp"

namespace VN
{

/**
 * @brief スレッドなし実装（フォールバック）
 *
 * スレッドをサポートしないプラットフォーム用のフォールバック実装です。
 * この実装では、スレッド作成時に関数が即時実行され、非同期処理は行われません。
 */
class Thread : public Thread_Base
{
public:
    // スレッドのコピーは禁止
    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;

    /**
     * @brief デフォルトコンストラクタ
     */
    Thread() = default;

    /**
     * @brief コンストラクタ：関数オブジェクト + 引数を受け取り、即時実行
     *
     * この実装では、スレッドは作成されず、関数が即時実行されます。
     */
    template <typename Callable, typename... Args>
    explicit Thread(Callable&& f, Args&&... args)
    {
        // 呼び出す関数をバインドして保存
        _func = std::bind(std::forward<Callable>(f), std::forward<Args>(args)...);
        
        // 即時実行
        if (_func) {
            _func();
            _executed = true;
        }
    }

    /**
     * @brief デストラクタ
     */
    ~Thread() override = default;

    /**
     * @brief スレッド終了を待機（この実装では何もしない）
     */
    void join() override final
    {
        // 即時実行されるため、何もする必要がない
    }

    /**
     * @brief スレッドのデタッチ（この実装では何もしない）
     */
    void detach() override final
    {
        // 即時実行されるため、何もする必要がない
    }

    /**
     * @brief スレッドがまだ実行中かどうか
     * 
     * この実装では常にfalseを返します（即時実行されるため）
     */
    bool joinable() const override final
    {
        return false;
    }

private:
    // ユーザが指定した関数を保持する
    std::function<void()> _func;
    
    // 実行済みフラグ
    bool _executed = false;
};

/**
 * @brief 現在実行中のスレッドを操作するための名前空間
 */
namespace thisThread
{
    /**
     * @brief 指定したマイクロ秒だけスリープ (ブロック)
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

#endif  // HAL_THREAD_DISABLED_HPP