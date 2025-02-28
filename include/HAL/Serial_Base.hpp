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

#ifndef HAL_SERIAL_BASE_HPP
#define HAL_SERIAL_BASE_HPP

#include "Interface/Errors.hpp"
#include "TemplateLibrary/String.hpp"
#include "TemplateLibrary/ByteBuffer.hpp"
#include "Interface/Command.hpp"
#include "Config.hpp"

namespace VN
{

/// @brief The interface class used for serial communication.
class Serial_Base
{
public:
    Serial_Base(ByteBuffer& byteBuffer) : _byteBuffer(byteBuffer) {};
    virtual ~Serial_Base() {};

    // -------------------------------
    /*! @name Port Access */
    // -------------------------------

    using PortName = String<Config::Serial::PortNameMaxLength>;

    /// @brief Opens communication to a serial port. No bytes are to be transmitted.
    /// @param portName Name of the port to open. On Windows, an example is "COM13"
    /// @param baudRate The baud rate at which to communicate. On Windows, an example is 115200.
    virtual Error open(const PortName& portName, const uint32_t baudRate) noexcept = 0;

    /// @brief Closes communication to a serial port, leaving it available to be reopened. No data can be sent or read.
    virtual void close() noexcept = 0;

    /// @brief Checks if the baud rate is supported by this serial interface.
    /// @param baudRate Baud rate to test.
    virtual bool isSupportedBaudRate([[maybe_unused]] const uint32_t baudRate) const noexcept { return true; }

    /// @brief Changes baud rate for both reading and writing. Recommended to flush hardware buffer during this call.
    /// @param baudRate
    virtual Error changeBaudRate(const uint32_t baudRate) noexcept = 0;

    /// @brief Gets the port name of the connected port, if applicable.
    virtual std::optional<PortName> connectedPortName() const noexcept { return _isOpen ? std::make_optional(_portName) : std::nullopt; };

    /// @brief Gets the baud rate of the connected port, if applicable.
    virtual std::optional<uint32_t> connectedBaudRate() const noexcept { return _isOpen ? std::make_optional(_baudRate) : std::nullopt; };

    // -------------------------------
    /*! @name Port Read/Write */
    // -------------------------------

    /// @brief Gets data from the hardware buffer, populating it into the registered byteBuffer.
    virtual Error getData() noexcept = 0;

    /// @brief Sends the passed message over the serial port.
    /// @param message The message to send over the port.
    virtual Error send(const AsciiMessage& message) noexcept = 0;

protected:
    ByteBuffer& _byteBuffer;
    static const size_t _numBytesToReadPerGetData = Config::Serial::numBytesToReadPerGetData;
    bool _isOpen = false;
    PortName _portName;
    uint32_t _baudRate = 0;
};

}  // namespace VN

#endif  // HAL_SERIAL_BASE_HPP
