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

#ifndef HAL_THREAD_ESP32_HPP
#define HAL_THREAD_ESP32_HPP

#include <Arduino.h>
#include <functional>
#include <utility>
#include "HAL/Thread_Base.hpp"

#ifdef ESP32
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#endif

namespace VN
{

/**
 * @brief ESP32 + FreeRTOS を使ったスレッドクラス
 *
 * PC版の std::thread ライクなインターフェースを模倣しています。
 * - `join()` : スレッド終了まで待機
 * - `detach()` : デタッチ
 * - `joinable()` : スレッドが現在アクティブかどうか
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

#ifdef ESP32
        // 完了通知用のセマフォを作成
        _completionSemaphore = xSemaphoreCreateBinary();
        
        // FreeRTOSタスクを作成
        BaseType_t result = xTaskCreate(
            Thread::_runTrampoline,  // タスク関数
            "VN_Thread",             // タスク名
            4096,                    // スタックサイズ
            this,                    // パラメータ
            1,                       // 優先度
            &_taskHandle             // タスクハンドル
        );
        
        if (result != pdPASS) {
            // タスク作成失敗
            _taskHandle = nullptr;
            if (_completionSemaphore) {
                vSemaphoreDelete(_completionSemaphore);
                _completionSemaphore = nullptr;
            }
        }
#else
        // ESP32以外の場合は即時実行
        if (_func) {
            _func();
        }
#endif
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
        
#ifdef ESP32
        // セマフォの削除
        if (_completionSemaphore) {
            vSemaphoreDelete(_completionSemaphore);
            _completionSemaphore = nullptr;
        }
#endif
    }

    /**
     * @brief スレッド終了を待機
     */
    void join() override final
    {
#ifdef ESP32
        if (joinable() && _completionSemaphore)
        {
            // セマフォが解放されるまで待機
            xSemaphoreTake(_completionSemaphore, portMAX_DELAY);
            xSemaphoreGive(_completionSemaphore);  // 再度解放して他のwaitが動作するようにする
            
            _taskHandle = nullptr;
        }
#endif
    }

    /**
     * @brief スレッドのデタッチ
     */
    void detach() override final
    {
#ifdef ESP32
        if (joinable())
        {
            _taskHandle = nullptr;  // ハンドルを解放するだけ
        }
#endif
    }

    /**
     * @brief スレッドがまだ実行中かどうか
     */
    bool joinable() const override final
    {
#ifdef ESP32
        return _taskHandle != nullptr;
#else
        return false;
#endif
    }

private:
#ifdef ESP32
    // スレッド実行時に呼び出される静的関数
    static void _runTrampoline(void* arg)
    {
        auto self = static_cast<Thread*>(arg);
        if (self && self->_func)
        {
            self->_func();
            
            // 完了を通知
            if (self->_completionSemaphore) {
                xSemaphoreGive(self->_completionSemaphore);
            }
        }
        
        // タスク終了
        vTaskDelete(NULL);
    }

    // FreeRTOSタスクハンドル
    TaskHandle_t _taskHandle = nullptr;
    
    // 完了通知用セマフォ
    SemaphoreHandle_t _completionSemaphore = nullptr;
#endif

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
     */
    inline void sleepFor(const Microseconds sleepDuration) noexcept
    {
#ifdef ESP32
        // FreeRTOSのディレイ関数を使用
        const uint64_t totalUs = static_cast<uint64_t>(sleepDuration.count());
        const uint32_t ms = static_cast<uint32_t>(totalUs / 1000ULL);
        
        if (ms > 0) {
            vTaskDelay(pdMS_TO_TICKS(ms));
        } else {
            // 1ms未満の場合はArduinoの関数を使用
            delayMicroseconds(static_cast<uint32_t>(totalUs));
        }
#else
        // ESP32以外の場合はArduinoの関数を使用
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
#endif
    }
}  // namespace thisThread

}  // namespace VN

#endif  // HAL_THREAD_ESP32_HPP