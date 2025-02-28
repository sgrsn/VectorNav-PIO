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

#include "Implementation/CommandProcessor.hpp"
#include <cstdint>
#include <optional>
#include "Debug.hpp"
#include "Interface/Command.hpp"
#include "TemplateLibrary/String.hpp"

namespace VN
{

CommandProcessor::RegisterCommandReturn CommandProcessor::registerCommand(Command* pCommand, const Microseconds timeoutThreshold) noexcept
{  // Should only be called on main thread
    if (pCommand->isAwaitingResponse()) { return RegisterCommandReturn{RegisterCommandReturn::Error::CommandResent, AsciiMessage{}}; }

    {
        LockGuard guard{_mutex};
        auto curr_time = now();
        while (!_cmdQueue.isEmpty())
        {
            const auto item = _cmdQueue.peek().value();
            if ((curr_time - item.cmd->getSentTime()) > item.timeoutThreshold) {
                _cmdQueue.get().value().cmd->setStale();
            } else { break; }
             
        }

        if (_cmdQueue.isFull()) { return RegisterCommandReturn{RegisterCommandReturn::Error::CommandQueueFull, AsciiMessage{}}; }
    }

    pCommand->prepareToSend();
    AsciiMessage messageToSend;
    sprintf(messageToSend.begin(), "$VN%s", pCommand->getCommandString().c_str());
    uint16_t crcValue = CalculateCRC((uint8_t*)messageToSend.c_str() + 1, messageToSend.length() - 1);
    sprintf(messageToSend.end(), "*%04X\r\n", crcValue);
    VN_DEBUG_1("TX: " + messageToSend);

    {
        LockGuard guard{_mutex};
        _cmdQueue.put({pCommand, timeoutThreshold});
    }
    return RegisterCommandReturn{RegisterCommandReturn::Error::None, messageToSend};
}

bool CommandProcessor::matchResponse(const AsciiMessage& response, const AsciiPacketProtocol::Metadata& metadata) noexcept
{  // Should be called on high-priority thread
    LockGuard guard{_mutex};
    while (!_cmdQueue.isEmpty())
    {
        const auto item = _cmdQueue.peek().value();
        if ((metadata.timestamp - item.cmd->getSentTime()) > item.timeoutThreshold) {
            _cmdQueue.get().value().cmd->setStale();
        } else { break; }
    }

    bool responseHasBeenMatched = false;
    VN_DEBUG_1("RX: " + response + "\t queue size: " + std::to_string(_cmdQueue.size()));
    if (StringUtils::startsWith(response, AsciiMessage("$VNERR,")))
    {
        if (Command::isMatchingError(response))
        {
            auto frontCommand = _cmdQueue.get();
            if (frontCommand.has_value())
            {
                // If we get a synchronous error, assume it is the oldest command.
                if (!frontCommand.value().cmd->matchResponse(response, metadata.timestamp))
                {
                    VN_ABORT();  // We just made sure it is a valid vnerr, should not be possible
                }
            }
            else { _asyncErrorQueuePush(AsyncError(Error::ReceivedUnexpectedMessage, response)); }
        }
        else
        {
            // Is an async error or is ill-formed
            AsciiMessage errorNumString = StringUtils::extractBetween(response, ',', '*');
            const auto errorNum = StringUtils::fromStringHex<uint8_t>(errorNumString.begin(), errorNumString.end());
            if (errorNum.has_value()) { _asyncErrorQueuePush(AsyncError(static_cast<Error>(errorNum.value()), response)); }
            else { _asyncErrorQueuePush(AsyncError(Error::ReceivedUnexpectedMessage, response)); }
        }
        return true;
    }
    else
    {
        while (!responseHasBeenMatched && !_cmdQueue.isEmpty())
        {
            bool validResponse = false;
            auto frontCommand = _cmdQueue.get();
            VN_ASSERT(frontCommand.has_value());  // The while loop validates that the command queue is not empty
            validResponse = (*frontCommand).cmd->matchResponse(response, metadata.timestamp);
            if (validResponse)
            {
                responseHasBeenMatched = true;
                VN_DEBUG_1("response matched.");
            }  // We don't need an else. Caller should be monitoring cmd object, and we want to limit errors thrown on high-priority thread.
            else { VN_DEBUG_1("response not matched."); }
        }
    }
    if (!responseHasBeenMatched)
    {
        _asyncErrorQueuePush(AsyncError(Error::ReceivedUnexpectedMessage, response));
        return true;
    }
    return false;
}

int CommandProcessor::queueSize() const noexcept
{
    LockGuard guard{_mutex};
    return _cmdQueue.size();
}

void CommandProcessor::popCommandFromQueueBack() noexcept
{
    LockGuard guard{_mutex};
    _cmdQueue.popBack();
}

std::optional<CommandProcessor::QueueItem> CommandProcessor::getFrontCommand() noexcept
{
    LockGuard guard{_mutex};
    return _cmdQueue.get();
}

}  // namespace VN
