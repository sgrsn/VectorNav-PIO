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

#ifndef INTERFACE_EXCEPTIONS_HPP
#define INTERFACE_EXCEPTIONS_HPP

#include <stdint.h>
#include <cstring>
#include "TemplateLibrary/String.hpp"
#include "Config.hpp"

#define DEBUG_MESSAGE_ENABLE true

namespace VN
{

enum class Error : uint16_t
{
    None = 0,
    // Sensor VN Errors
    HardFault = 0x01,
    SerialBufferOverflow = 0x02,
    InvalidChecksum = 0x03,
    InvalidCommand = 0x04,
    NotEnoughParameters = 0x05,
    TooManyParameters = 0x06,
    InvalidParameter = 0x07,
    InvalidRegister = 0x08,
    UnauthorizedAccess = 0x09,
    WatchdogReset = 0x0A,
    OutputBufferOverflow = 0x0B,
    InsufficientBaudRate = 0x0C,
    ErrorBufferOverflow = 0xFF,

    // CommandProcessorErrors
    CommandResent = 301,
    CommandQueueFull = 302,
    ResponseTimeout = 303,
    ReceivedUnexpectedMessage = 304,

    // SensorErrors
    MeasurementQueueFull = 600,
    PrimaryBufferFull = 601,
    MessageSubscriberCapacityReached = 603,
    ReceivedInvalidResponse = 604,

    // SerialErrors
    InvalidPortName = 700,
    AccessDenied = 701,
    SerialPortClosed = 702,
    UnsupportedBaudRate = 703,
    SerialReadFailed = 705,
    SerialWriteFailed = 706,
    UnexpectedSerialError = 799,

    // PacketSyncErrors
    SkippedByteBufferFull = 800,
    ReceivedByteBufferFull = 801,

};

inline static const char* errorCodeToString(Error error)
{
    switch (error)
    {
        case Error::None:
            return "None";
        case Error::HardFault:
            return "HardFault";
        case Error::SerialBufferOverflow:
            return "SerialBufferOverflow";
        case Error::InvalidChecksum:
            return "InvalidChecksum";
        case Error::InvalidCommand:
            return "InvalidCommand";
        case Error::NotEnoughParameters:
            return "NotEnoughParameters";
        case Error::TooManyParameters:
            return "TooManyParameters";
        case Error::InvalidParameter:
            return "InvalidParameter";
        case Error::InvalidRegister:
            return "InvalidRegister";
        case Error::UnauthorizedAccess:
            return "UnauthorizedAccess";
        case Error::WatchdogReset:
            return "WatchdogReset";
        case Error::OutputBufferOverflow:
            return "OutputBufferOverflow";
        case Error::InsufficientBaudRate:
            return "InsufficientBaudRate";
        case Error::ErrorBufferOverflow:
            return "ErrorBufferOverflow";
        case Error::PrimaryBufferFull:
            return "PrimaryBufferFull";
        case Error::CommandResent:
            return "CommandResent";
        case Error::CommandQueueFull:
            return "CommandQueueFull";
        case Error::ResponseTimeout:
            return "ResponseTimeout";
        case Error::ReceivedUnexpectedMessage:
            return "ReceivedUnexpectedMessage";
        case Error::ReceivedInvalidResponse:
            return "ReceivedInvalidResponse";
        case Error::MeasurementQueueFull:
            return "MeasurementQueueFull";
        case Error::InvalidPortName:
            return "InvalidPortName";
        case Error::AccessDenied:
            return "AccessDenied";
        case Error::SerialPortClosed:
            return "SerialPortClosed";
        case Error::UnsupportedBaudRate:
            return "UnsupportedBaudRate";
        case Error::UnexpectedSerialError:
            return "UnexpectedSerialError";
        case Error::SerialReadFailed:
            return "SerialReadFailed";
        case Error::SerialWriteFailed:
            return "SerialWriteFailed";
        case Error::MessageSubscriberCapacityReached:
            return "MessageSubscriberCapacityReached";
        case Error::SkippedByteBufferFull:
            return "SkippedByteBufferFull";
        case Error::ReceivedByteBufferFull:
            return "ReceivedByteBufferFull";
        default:
            return "Unknown error code.";
    }
}

inline std::ostream& operator<<(std::ostream& outStream, const Error& error) noexcept
{
#if DEBUG_MESSAGE_ENABLE
    outStream << static_cast<uint16_t>(error) << ": " << errorCodeToString(error);
#else
    outStream << static_cast<uint16_t>(error);
#endif
    return outStream;
}

template <typename EnumType, EnumType... Values>
class EnumCheck;

template <typename EnumType>
class EnumCheck<EnumType>
{
public:
    template <typename IntType>
    static bool constexpr is_value(IntType)
    {
        return false;
    }
};

template <typename EnumType, EnumType V, EnumType... Next>
class EnumCheck<EnumType, V, Next...> : private EnumCheck<EnumType, Next...>
{
    using super = EnumCheck<EnumType, Next...>;

public:
    template <typename IntType>
    static bool constexpr is_value(IntType v)
    {
        return v == static_cast<IntType>(V) || super::is_value(v);
    }
};

struct AsyncError
{
    using Message = String<Config::Errors::asyncErrorMessageCapacity>;
    AsyncError() {};
    AsyncError(const Error error) : error(error) {};
    AsyncError(const Error error, const Message& message) : error(error), message(message) {};
    Error error;
    Message message;

    bool operator==(const AsyncError& other) const { return error == other.error && message == other.message; }
    bool operator!=(const AsyncError& other) const { return !(*this == other); }
};

using VnErr_Synchronous =
    EnumCheck<Error, Error::SerialBufferOverflow, Error::InvalidChecksum, Error::InvalidCommand, Error::NotEnoughParameters, Error::TooManyParameters,
              Error::InvalidParameter, Error::InvalidRegister, Error::UnauthorizedAccess, Error::InsufficientBaudRate>;

using VnErr_Asynchronous = EnumCheck<Error, Error::HardFault, Error::WatchdogReset, Error::OutputBufferOverflow, Error::ErrorBufferOverflow>;

inline bool SensorException_Is_Asynchronous(Error error) { return VnErr_Asynchronous::is_value(error); }

inline bool SensorException_Is_Synchronous(Error error) { return VnErr_Synchronous::is_value(error); }

}  // namespace VN

#endif  // INTERFACE_EXCEPTIONS_HPP
