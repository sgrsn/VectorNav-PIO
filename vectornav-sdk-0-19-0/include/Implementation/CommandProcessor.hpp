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

#ifndef IMPLEMENTATION_COMMANDPROCESSOR_HPP
#define IMPLEMENTATION_COMMANDPROCESSOR_HPP

#include <functional>
#include <memory>  // Used for shared_ptr
#include <assert.h>
#include "Config.hpp"
#include "HAL/Mutex.hpp"
#include "TemplateLibrary/String.hpp"
#include "TemplateLibrary/Queue.hpp"
#include "Implementation/CoreUtils.hpp"
#include "Interface/Command.hpp"
#include "Interface/Errors.hpp"
#include "Implementation/AsciiPacketProtocol.hpp"

namespace VN
{

/// @brief The CommandProcessor class is the handshake between the user-facing code (such as Sensor) and the communication code interfacing with the sensor
/// (such as Serial and PacketProcessor). Consequently, it exists on multiple threads (all exists on the main thread except for matchReponse, which is called
/// from the high-priority thread) with an internal queue to handle the cross-thread communication. Its chief responsibility is to facilitate the communication
/// of commands between the user and the sensor, tracking received responses (sent via matchResponse) and correlating them with known-sent commands.
class CommandProcessor
{
public:
    using AsyncErrorQueuePush = std::function<void(AsyncError&&)>;
    CommandProcessor(AsyncErrorQueuePush asyncErrorQueuePush) : _asyncErrorQueuePush(asyncErrorQueuePush) {}

    struct RegisterCommandReturn
    {
        enum class Error
        {
            None,
            CommandQueueFull,
            CommandResent
        } error;
        AsciiMessage message;
    };
    
    struct QueueItem {
        Command* cmd;
        Microseconds timeoutThreshold = Config::CommandProcessor::commandRemovalTimeoutLength;
    };
    
    RegisterCommandReturn registerCommand(Command* pCommand, const Microseconds timeoutThreshold = Config::CommandProcessor::commandRemovalTimeoutLength) noexcept;

    bool matchResponse(const AsciiMessage& response, const AsciiPacketProtocol::Metadata& metadata) noexcept;

    int queueSize() const noexcept;
    int queueCapacity() const noexcept;
    void popCommandFromQueueBack() noexcept;
    std::optional<QueueItem> getFrontCommand() noexcept;


private:
    AsyncErrorQueuePush _asyncErrorQueuePush = nullptr;

    Queue<QueueItem, Config::CommandProcessor::commandProcQueueCapacity> _cmdQueue{};
    mutable Mutex _mutex;
};

}  // namespace VN

#endif  // IMPLEMENTATION_COMMANDPROCESSOR_HPP
