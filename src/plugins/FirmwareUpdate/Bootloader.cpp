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

#include "Bootloader.hpp"
#include <cstdint>
#include <algorithm>

#include "Interface/Command.hpp"
#include "Interface/Commands.hpp"
#include "Interface/Errors.hpp"
#include "Interface/Sensor.hpp"
#include "Interface/Registers.hpp"
#include "HAL/Timer.hpp"
#include "TemplateLibrary/String.hpp"
#include "TemplateLibrary/Vector.hpp"
#include "Config.hpp"

namespace VN
{
namespace Bootloader
{

class BootloaderDispatcher : public PacketDispatcher
{
public:
    BootloaderDispatcher() : PacketDispatcher{Vector<uint8_t, SYNC_BYTE_CAPACITY>{'V'}} {};
    virtual FindPacketRetVal findPacket(const ByteBuffer& byteBuffer, const size_t syncByteIndex) noexcept
    {
        const String<23> blMsgMatch = "VectorNav Bootloader v.";

        if (byteBuffer.size() < blMsgMatch.length())
        {
            // There's not enough data to have the full message, don't bother checking
            return {FindPacketRetVal::Validity::Incomplete, blMsgMatch.length()};
        }
        for (uint8_t i = 0; i < blMsgMatch.length(); ++i)
        {
            if (byteBuffer.peek_unchecked(syncByteIndex + i) != blMsgMatch[i]) { return {FindPacketRetVal::Validity::Invalid, 0}; }
        }
        return {FindPacketRetVal::Validity::Valid, blMsgMatch.length()};
    }

    virtual void dispatchPacket(const ByteBuffer& byteBuffer, const size_t syncByteIndex) noexcept
    {
        size_t byteBufferSize = byteBuffer.size();
        const size_t blMsgLength = 34;  // Enough length for "VectorNav Bootloader v.XX.XX.XX.XX"
        size_t messageToPrintSize = std::min(byteBufferSize, blMsgLength);
        for (uint8_t i = 0; i < messageToPrintSize; ++i) { std::cout << byteBuffer.peek_unchecked(syncByteIndex + i); }
        std::cout << std::endl;
    }
};

namespace
{

Error _sendRecord(Sensor* sensor, const AsciiMessage& currentRecord);
Error _getSensorError(const AsciiMessage& inMsg);
}  // namespace

bool tryEnterBootloader(Sensor* sensor, const Sensor::BaudRate firmwareBaudRate, const Sensor::BaudRate bootloaderBaudRate)
{
    std::cout << "Entering bootloader...\n";
    bool enteringFailed = false;

    Command enterBootloader("FWU", 3);
    sensor->sendCommand(&enterBootloader, Sensor::SendCommandBlockMode::BlockWithRetry, 6s);
    // No need to sleep, because worst case scenario we are sending spaces too early.
    if (autoconfigureBootloader(sensor, bootloaderBaudRate))
    {
        sensor->changeHostBaudRate(firmwareBaudRate);  // If we didn't make it into the bootloader, assume we're talking to the firmware
        sensor->reset();
        enteringFailed = true;
    }

    sensor->deregisterSkippedByteBuffer();
    if (enteringFailed) { std::cout << "Failed to enter bootloader.\n"; }
    else { std::cout << "Bootloader entered.\n"; }

    return enteringFailed;
}

bool autoconfigureBootloader(Sensor* sensor, const Sensor::BaudRate bootloaderBaudRate)
{
    ByteBuffer _skippedByteBuffer{200};

    BootloaderDispatcher blDispatch{};
    PacketSynchronizer packetSync{_skippedByteBuffer, nullptr, 10};

    packetSync.addDispatcher(&blDispatch);
    sensor->registerSkippedByteBuffer(&_skippedByteBuffer);

    const AsciiMessage autobaudSequence = "                            ";  // Only 8 needed, but it's safe to send extra
    VN::Error baudRateError = sensor->changeHostBaudRate(bootloaderBaudRate);
    if (baudRateError != VN::Error::None)
    {
        std::cout << "Error " << baudRateError << " encountered when attempting to change the host baud rate." << std::endl;
        sensor->deregisterSkippedByteBuffer();
        return true;
    }

    // Over 35 trials, 8 took more than 10 retries, 1 took 34 retries
    // This will only take a max of 4 seconds
    const uint8_t numAllowedRetries = 40;
    for (int attemptNumber = 0; attemptNumber <= numAllowedRetries; ++attemptNumber)
    {
        VN::Error serialError = sensor->serialSend(autobaudSequence);
        if (serialError != VN::Error::None)
        {
            std::cout << "Error " << serialError << " encountered when configuring the bootloader on attempt " << (attemptNumber + 1) << ": " << std::endl;
            continue;
        }
        Timer timer(50ms);
        timer.start();
        while (!timer.hasTimedOut())
        {
#if (!THREADING_ENABLE)
            sensor->loadMainBufferFromSerial();
            while (!sensor->processNextPacket()) {}
#endif
            if (packetSync.dispatchNextPacket()) {}
            if (packetSync.getValidPacketCount({'V'}) > 0)
            {
                sensor->deregisterSkippedByteBuffer();
                return false;
            }
        }
    }
    sensor->deregisterSkippedByteBuffer();
    return true;  // Timed out
}

FailureMode sendRecords(Sensor* sensor, InputFile& firmwareStream, const size_t numLinesInFirmware)
{
    AsciiMessage progressBar;
    std::fill_n(progressBar.begin(), 100, '-');

    size_t lineNum = 0;
    uint8_t percentComplete = 0;

    std::printf("\r[%10s] %03u%%", progressBar.c_str(), percentComplete);
    std::cout << std::flush;
    while (lineNum < numLinesInFirmware)
    {
        if ((lineNum * 100 / numLinesInFirmware) > percentComplete)
        {
            percentComplete = lineNum * 100 / numLinesInFirmware;
            progressBar.at(percentComplete - 1) = '#';
            std::printf("\r[%10s] %03u%%", progressBar.c_str(), percentComplete);
            std::cout << std::flush;
        }

        AsciiMessage currentLine;
        if (firmwareStream.getLine(currentLine.begin(), currentLine.capacity()))
        {
            std::cout << "Failed to get line.\n";
            return FailureMode::Abort;
        }
        Bootloader::Error error = _sendRecord(sensor, currentLine);
        switch (error)
        {
            case (Error::None):
            {
                lineNum++;
                break;
            }
            case (Error::CommError):
            {
                // Retry the same line
                std::cout << "Error " << error << " encountered while loading the firmware on line " << lineNum << "." << std::endl;
                // TODO: VN_ABORT or return FailureMode...?
                VN_ABORT();  // Todo: No mechanism exists to go back one line and resend. Minor rewrite necessary.
            }
            case (Error::InvalidProgramCRC):
            case (Error::InvalidProgramSize):
            {
                std::cout << "Error " << error << " encountered while loading the firmware on line " << lineNum << "." << std::endl;
                sensor->reset();
                return FailureMode::Abort;
            }
            case (Error::Timeout):
            {
                std::cout << "Error " << error << " encountered while loading the firmware on line " << lineNum << "." << std::endl;
                return FailureMode::Retry;
            }
            case (Error::InvalidCommand):
            case (Error::InvalidRecordType):
            case (Error::InvalidByteCount):
            case (Error::InvalidHexFile):
            case (Error::DecryptionError):
            case (Error::InvalidBlockCRC):
            case (Error::InvalidMemoryAddress):
            case (Error::MaxRetryCount):
            case (Error::Reserved):
            {
                std::cout << "Error " << error << " encountered while loading the firmware on line " << lineNum << "." << std::endl;
                return FailureMode::Abort;
            }
            default:
                std::cout << "Error code not recognized" << std::endl;
                VN_ABORT();
        }
    }
    progressBar.at(99) = '#';
    std::printf("\r[%10s] %03d%%\n", progressBar.c_str(), 100);
    std::cout << std::flush;
    return FailureMode::None;
}

bool exitBootloader(Sensor* sensor)
{
    VN::Error latestError = sensor->reset();
    if (latestError != VN::Error::None)
    {
        std::cout << "Error " << latestError << " encountered when resetting the sensor." << std::endl;
        ;
        return true;
    }
    return false;
}

namespace
{
Bootloader::Error _sendRecord(Sensor* sensor, const AsciiMessage& currentLine)
{
    const auto record = StringUtils::extractAfter(currentLine, ':');
    Command programCommand("BLD," + record, 3);  // Expecting response with "BLD"
    sensor->sendCommand(&programCommand, Sensor::SendCommandBlockMode::Block, 6s);
    Bootloader::Error error = _getSensorError(programCommand.getResponse());
    return error;
}

Bootloader::Error _getSensorError(const AsciiMessage& inMsg)
{
    if (StringUtils::startsWith(inMsg, "$VNBLD,"))
    {
        auto errorString = StringUtils::extractBetween(inMsg, ',', '*');
        auto errorType = StringUtils::fromStringHex<uint8_t>(errorString.begin(), errorString.end()).value();
        if (errorType < static_cast<uint8_t>(Error::Reserved)) { return static_cast<Bootloader::Error>(errorType); }
        return Error::None;
    }
    return Error::None;
}

}  // namespace
}  // namespace Bootloader
}  // namespace VN
