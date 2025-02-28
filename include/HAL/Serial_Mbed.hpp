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

#ifndef HAL_SERIALARDUINO_HPP
#define HAL_SERIALARDUINO_HPP

#include <Arduino.h>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <array>
#include <algorithm>

#include "HAL/Serial_Base.hpp"
#include "Interface/Errors.hpp"
#include "Serial_Base.hpp"

namespace VN
{

/**
 * @brief Arduino 環境向けシリアル通信クラス
 *
 * Linux 版の Serial_Linux.hpp を参考に、HardwareSerial を利用して実装。
 *
 * - ポート名には "Serial", "Serial1", "Serial2", ... といった文字列を使用し、
 *   open() 時にどの HardwareSerial を使うかを振り分けます。
 * - ポート名やサポートするボーレートの範囲などは必要に応じて調整してください。
 */
class Serial : public Serial_Base
{
public:
    using Serial_Base::Serial_Base;

    // ***********
    // Port access
    // ***********
    Error open(const PortName& portName, const uint32_t baudRate) noexcept override final;
    void close() noexcept override;
    bool isSupportedBaudRate(const uint32_t baudRate) const noexcept override;
    Error changeBaudRate(const uint32_t baudRate) noexcept override final;

    // ***************
    // Port read/write
    // ***************
    Error getData() noexcept override final;
    Error send(const AsciiMessage& message) noexcept override final;

private:
    // Arduino 環境で使用するシリアルポインタ
    HardwareSerial* _hwSerial = nullptr;

    // Linux 実装で使っていたファイルディスクリプタの代わりに、開閉状態のみを管理
    // int _portHandle = 0; -> 不要

    // ここでは任意のボーレートを許容するため、簡易的に true を返す
    bool _isSupportedBaudRate(const uint32_t baudRate) const noexcept;

    // ***************
    // Port read/write
    // ***************
    void _flush();
    std::array<uint8_t, _numBytesToReadPerGetData> _inputBuffer = {0};
};

// ######################
//     Implementation
// ######################

// --------------------
// Port access
// --------------------
inline Error Serial::open(const PortName& portName, const uint32_t baudRate) noexcept
{
    if (_isOpen)
    {
        // すでに開いていれば閉じる
        close();
    }

    // ポート名を判定して HardwareSerial を設定（以下は例。必要に応じて増やしてください）
    if (portName == "Serial1")
    {
        _hwSerial = &Serial1;
    }
    else if (portName == "Serial2")
    {
        _hwSerial = &Serial2;
    }
    else if (portName == "Serial3")
    {
        _hwSerial = &Serial3;
    }
    else
    {
        return Error::InvalidPortName;
    }

    // ボーレートがサポート対象か判定
    if (!isSupportedBaudRate(baudRate))
    {
        return Error::UnsupportedBaudRate;
    }

    // Teensy (Arduino) 用にシリアル開始
    _hwSerial->begin(baudRate);

    // 開始直後は受信バッファが残っている可能性もあるのでクリア
    _flush();

    _portName = portName;
    _baudRate = baudRate;
    _isOpen = true;

    return Error::None;
}

inline void Serial::close() noexcept
{
    if (_hwSerial && _isOpen)
    {
        _hwSerial->end();
        _hwSerial = nullptr;
    }
    _isOpen = false;
}

inline bool Serial::isSupportedBaudRate(const uint32_t baudRate) const noexcept
{
    return _isSupportedBaudRate(baudRate);
}

inline bool Serial::_isSupportedBaudRate(const uint32_t /*baudRate*/) const noexcept
{
    // todo: 必要に応じてサポートするボーレートを追加
    return true;
}

inline Error Serial::changeBaudRate(const uint32_t baudRate) noexcept
{
    if (!_isOpen || !_hwSerial)
    {
        return Error::SerialPortClosed;
    }

    if (!isSupportedBaudRate(baudRate))
    {
        return Error::UnsupportedBaudRate;
    }

    // いったん終了して再度開始
    _hwSerial->end();
    _hwSerial->begin(baudRate);

    _baudRate = baudRate;
    return Error::None;
}

// --------------------
// Port read/write
// --------------------
inline Error Serial::getData() noexcept
{
    if (!_isOpen || !_hwSerial)
    {
        return Error::SerialPortClosed;
    }

    // 受信バイト数を確認
    int availableBytes = _hwSerial->available();
    if (availableBytes <= 0)
    {
        return Error::None;  // データが来ていない
    }

    // 今回読み取るバイト数を決定（_inputBuffer のサイズを超えないように）
    size_t toRead = std::min<size_t>(availableBytes, _inputBuffer.size());

    // 実際に読み取る
    size_t readCount = 0;
    for (; readCount < toRead; ++readCount)
    {
        int c = _hwSerial->read();
        if (c < 0)
        {
            // 読み取り失敗ならループ終了
            break;
        }
        _inputBuffer[readCount] = static_cast<uint8_t>(c);
    }

    // SDK 側のバッファ (_byteBuffer) に格納
    if (_byteBuffer.put(_inputBuffer.data(), readCount))
    {
        return Error::PrimaryBufferFull;  // バッファあふれ
    }

    return Error::None;
}

inline Error Serial::send(const AsciiMessage& message) noexcept
{
    if (!_isOpen || !_hwSerial)
    {
        return Error::SerialPortClosed;
    }

    // Arduino の write は size_t 型の書き込みバイト数を返す
    size_t written = _hwSerial->write(
        reinterpret_cast<const uint8_t*>(message.c_str()),
        message.length());

    if (written < message.length())
    {
        // 書き込みが不完全ならエラー
        return Error::SerialWriteFailed;
    }

    return Error::None;
}

inline void Serial::_flush()
{
    if (_hwSerial)
    {
        // Arduino の flush() は送信バッファが空になるまで待つ
        _hwSerial->flush();

        // 受信バッファのクリア
        while (_hwSerial->available() > 0)
        {
            (void)_hwSerial->read();
        }
    }
}

}  // namespace VN

#endif  // HAL_SERIALARDUINO_HPP
