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

#ifndef FIRMWAREUPDATE_BOOTLOADER_HPP
#define FIRMWAREUPDATE_BOOTLOADER_HPP

#include "HAL/File.hpp"
#include "Interface/Sensor.hpp"

namespace VN
{
namespace Bootloader
{
bool tryEnterBootloader(Sensor* sensor, const Sensor::BaudRate firmwareBaudRate, const Sensor::BaudRate bootloaderBaudRate);
bool autoconfigureBootloader(Sensor* sensor, const Sensor::BaudRate bootloaderBaudRate);
enum class FailureMode
{
    None,
    Retry,
    Abort
};
FailureMode sendRecords(Sensor* sensor, InputFile& filePath, const size_t numLinesInFirmware);
bool exitBootloader(Sensor* sensor);

enum class Error : uint8_t
{
    None = 0,
    InvalidCommand = 0x01,
    InvalidRecordType = 0x02,
    InvalidByteCount = 0x03,
    InvalidMemoryAddress = 0x04,
    CommError = 0x05,
    InvalidHexFile = 0x06,
    DecryptionError = 0x07,
    InvalidBlockCRC = 0x08,
    InvalidProgramCRC = 0x09,
    InvalidProgramSize = 0x0A,
    MaxRetryCount = 0x0B,
    Timeout = 0x0C,
    Reserved = 0x0D
};

inline static const char* errorCodeToString(Error error)
{
    switch (error)
    {
        case Error::None:
            return "None";
        case Error::InvalidCommand:
            return "InvalidCommand";
        case Error::InvalidRecordType:
            return "InvalidRecordType";
        case Error::InvalidByteCount:
            return "InvalidByteCount";
        case Error::InvalidMemoryAddress:
            return "InvalidMemoryAddress";
        case Error::CommError:
            return "CommError";
        case Error::InvalidHexFile:
            return "InvalidHexFile";
        case Error::DecryptionError:
            return "DecryptionError";
        case Error::InvalidBlockCRC:
            return "InvalidBlockCRC";
        case Error::InvalidProgramCRC:
            return "InvalidProgramCRC";
        case Error::InvalidProgramSize:
            return "InvalidProgramSize";
        case Error::MaxRetryCount:
            return "MaxRetryCount";
        case Error::Timeout:
            return "Timeout";
        case Error::Reserved:
            return "Reserved";
        default:
            return "Unknown error code.";
    }
}

inline std::ostream& operator<<(std::ostream& outStream, const Error& error) noexcept
{
#if DEBUG_MESSAGE_ENABLE
    outStream << static_cast<uint8_t>(error) << ": " << errorCodeToString(error);
#else
    outStream << static_cast<uint8_t>(error);
#endif
    return outStream;
}

}  // namespace Bootloader

};  // namespace VN
#endif  // #ifndef FIRMWAREUPDATE_BOOTLOADER_HPP
