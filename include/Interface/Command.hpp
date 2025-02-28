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

#ifndef INTERFACE_COMMAND_HPP
#define INTERFACE_COMMAND_HPP

#include "Implementation/MeasurementDatatypes.hpp"
#include "TemplateLibrary/String.hpp"
#include "Interface/Errors.hpp"
#include "HAL/Mutex.hpp"
#include "HAL/Timer.hpp"

namespace VN
{

using AsciiMessage = String<Config::CommandProcessor::messageMaxLength>;

/// @brief The Command class is the mechanism to pass all commands to the unit (including Read and Write Register) and track the unit's respective response.
class Command
{
public:
    Command(const AsciiMessage& commandString = "", const uint8_t numCharToMatch = 3) : _commandString(commandString), _numCharToMatch(numCharToMatch) {}

    virtual ~Command() = default;

    Command(const Command& other)
    {
        LockGuard lock(other._mutex);
        _commandString = other._commandString;
        _numCharToMatch = other._numCharToMatch;
        _awaitingResponse = other._awaitingResponse;
        _responseMatched = other._responseMatched;
        _sentTime = other._sentTime;
        _responseTime = other._responseTime;
    }

    Command& operator=(const Command& other)
    {
        if (this == &other) { return *this; }

        LockGuard thisLock(_mutex);
        LockGuard otherLock(other._mutex);

        _commandString = other._commandString;
        _numCharToMatch = other._numCharToMatch;
        _awaitingResponse = other._awaitingResponse;
        _responseMatched = other._responseMatched;
        _sentTime = other._sentTime;
        _responseTime = other._responseTime;
        return *this;
    }

    Command(Command&& other) noexcept
    {
        LockGuard lock(other._mutex);

        _commandString = std::move(other._commandString);
        _numCharToMatch = other._numCharToMatch;
        _awaitingResponse = other._awaitingResponse;
        _responseMatched = other._responseMatched;
        _sentTime = other._sentTime;
        _responseTime = other._responseTime;
    }

    Command& operator=(Command&& other) noexcept
    {
        LockGuard lock(other._mutex);

        _commandString = std::move(other._commandString);
        _numCharToMatch = other._numCharToMatch;
        _awaitingResponse = other._awaitingResponse;
        _responseMatched = other._responseMatched;
        _sentTime = other._sentTime;
        _responseTime = other._responseTime;

        return *this;
    }

    // -------------------------------
    /*! @name Used by Caller */
    // -------------------------------

    /// @brief The SDK is awaiting a response from the unit for this command. The command queue has NOT yet popped this command.
    bool isAwaitingResponse() const noexcept;

    /// @brief The SDK has  received a matching response from the unit.
    bool hasValidResponse() const noexcept;

    /// @brief Gets the response parsed as an error, if applicable.
    std::optional<Error> getError() const noexcept;

    /// @brief Gets the response.
    AsciiMessage getResponse() const noexcept;

    /// @brief Gets the time the command was sent to the unit.
    time_point getSentTime() const noexcept
    {
        LockGuard lock(_mutex);
        return _sentTime;
    }

    /// @brief Gets the time a response was received by the SDK.
    time_point getResponseTime() const noexcept
    {
        LockGuard lock(_mutex);
        return _responseTime;
    }

    void setStale() noexcept {
        LockGuard lock(_mutex);
        _awaitingResponse = false;
        _responseMatched = false;
    }
    
    // -------------------------------
    /*! @name Used by Command Processor */
    // -------------------------------

    /// @brief Formats the command to send as a string.
    virtual AsciiMessage getCommandString() noexcept;

    /// @brief Tests a possible response, saving it as the valid response if it matches. A synchronous VNERR is always a matching response.
    /// @param responseToCheck The unit's response to attempt to match.
    /// @param timestamp The timestamp that will be saved as the response time if the response matches.
    /// @return Response was matched.
    virtual bool matchResponse(const AsciiMessage& responseToCheck, const time_point timestamp) noexcept;

    /// @brief Sets necessary flags to send this command to the unit.
    void prepareToSend() noexcept;

    /// @brief Tests if the error is matching (i.e. is synchronous)
    /// @param errIn Error to check.
    /// @return Error matches.
    static bool isMatchingError(const AsciiMessage& errIn) noexcept;

protected:
    AsciiMessage _commandString;
    uint8_t _numCharToMatch;
    bool _awaitingResponse = false;
    bool _responseMatched = false;
    mutable Mutex _mutex;
    time_point _sentTime;
    time_point _responseTime;
    bool _hasValidResponse() const noexcept;
    static std::optional<uint16_t> _getErrorValue(const AsciiMessage& errIn) noexcept;
};
}  // namespace VN

#endif  // INTERFACE_COMMAND_HPP
