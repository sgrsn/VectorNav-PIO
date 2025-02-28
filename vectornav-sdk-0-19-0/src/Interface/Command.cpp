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


#include "Interface/Command.hpp"

#include "Debug.hpp"
#include "TemplateLibrary/String.hpp"

namespace VN
{
bool Command::isAwaitingResponse() const noexcept
{
    LockGuard lock(_mutex);
    return _awaitingResponse;
}

bool Command::hasValidResponse() const noexcept
{
    LockGuard lock(_mutex);
    return _hasValidResponse();
}

AsciiMessage Command::getResponse() const noexcept
{
    LockGuard lock(_mutex);
    if (!_hasValidResponse()) { return ""; }
    else { return _commandString; }
}

std::optional<Error> Command::getError() const noexcept
{
    LockGuard lock(_mutex);
    if (!_hasValidResponse()) { return std::nullopt; }
    if (isMatchingError(_commandString))
    {
        auto errorValue = _getErrorValue(_commandString);
        if (!errorValue.has_value()) { return std::nullopt; }
        return std::make_optional(static_cast<Error>(errorValue.value()));
    }
    else { return std::nullopt; }
}

AsciiMessage Command::getCommandString() noexcept
{
    LockGuard lock(_mutex);
    if (_hasValidResponse()) { return ""; }
    return _commandString;
}

std::optional<uint16_t> Command::_getErrorValue(const AsciiMessage& errIn) noexcept
{
    const uint8_t commaIdx = 6;  //$VNERR,
    if ((errIn.length() < 8) || errIn.at(commaIdx) != ',') { return false; }

    const size_t astereskIndex = errIn.find('*', commaIdx);
    if (astereskIndex == AsciiMessage::npos) { return false; }

    AsciiMessage errorNumString = StringUtils::extractBetween(errIn, ',', '*');
    //  errIn.begin() + commaIdx, errIn.begin() + astereskIndex;
    return StringUtils::fromStringHex<uint16_t>(errorNumString.begin(), errorNumString.end());
}

bool Command::isMatchingError(const AsciiMessage& errIn) noexcept
{
    if (!StringUtils::startsWith(errIn, "$VNERR,")) { return false; }
    auto errorValue = _getErrorValue(errIn);
    if (!errorValue.has_value()) { return false; }
    return VnErr_Synchronous::is_value(static_cast<Error>(errorValue.value()));
}

bool Command::matchResponse(const AsciiMessage& responseToCheck, const time_point timestamp) noexcept
{
    LockGuard lock(_mutex);
    _awaitingResponse = false;
    _responseMatched = false;
    AsciiMessage stringToMatch{};
    std::snprintf(stringToMatch.begin(), 3 + 1 + _numCharToMatch, "$VN%s", _commandString.c_str());
    if (StringUtils::startsWith(responseToCheck, stringToMatch)) { _responseMatched = true; }
    else
    {
        if (isMatchingError(responseToCheck)) { _responseMatched = true; }
        else { VN_DEBUG_1("response NOT matched.\n Expected response:\t" + stringToMatch + "\nReceived response:\t" + responseToCheck); }
    }
    if (_responseMatched)
    {
        _commandString = responseToCheck;
        _responseTime = timestamp;
    }
    return _responseMatched;
}

void Command::prepareToSend() noexcept
{
    LockGuard lock(_mutex);
    _awaitingResponse = true;
    _responseMatched = false;
    _sentTime = now();
}

bool Command::_hasValidResponse() const noexcept
{
    // We shouldn't need to check awaiting response because if it is awaiting, the response should always be empty.
    return (!_awaitingResponse && _responseMatched);
}

}  // namespace VN
