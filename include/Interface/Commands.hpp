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

#ifndef INTERFACE_COMMANDS_HPP
#define INTERFACE_COMMANDS_HPP

#include "Interface/Command.hpp"
#include <cstdint>
#include <cstdio>

namespace VN
{

class WriteSettings : public Command
{
public:
    WriteSettings() : Command{"WNV"} {};
};

class RestoreFactorySettings : public Command
{
public:
    RestoreFactorySettings() : Command{"RFS"} {};
};

class Reset : public Command
{
public:
    Reset() : Command{"RST"} {};
};

class FirmwareUpdate : public Command
{
public:
    FirmwareUpdate() : Command("FWU") {};
};

class KnownMagneticDisturbance : public Command
{
public:
    enum class State : uint8_t
    {
        NotPresent = 0,
        Present = 1
    } state;

    KnownMagneticDisturbance(const State state) : Command{"KMD"}, state(state) {};

    AsciiMessage getCommandString() noexcept override
    {
        std::snprintf(_commandString.begin(), _commandString.capacity(), "KMD,%d", static_cast<uint8_t>(state));
        return _commandString;
    };
};

class KnownAccelerationDisturbance : public Command
{
public:
    enum class State : uint8_t
    {
        NotPresent = 0,
        Present = 1
    } state;

    KnownAccelerationDisturbance(const State state) : Command{"KAD"}, state(state) {};

    AsciiMessage getCommandString() noexcept override
    {
        std::snprintf(_commandString.begin(), _commandString.capacity(), "KAD,%d", static_cast<uint8_t>(state));
        return _commandString;
    };
};

class SetInitialHeading : public Command
{
public:
    SetInitialHeading(const float heading) : Command{"SIH"} { params.push_back(heading); };

    SetInitialHeading(const Ypr ypr) : Command{"SIH"}
    {
        params.push_back(ypr.yaw);
        params.push_back(ypr.pitch);
        params.push_back(ypr.roll);
    };

    SetInitialHeading(const Quat quat) : Command{"SIH"}
    {
        params.push_back(quat.vector[0]);
        params.push_back(quat.vector[1]);
        params.push_back(quat.vector[2]);
        params.push_back(quat.scalar);
    };

    Vector<float, 4> params;
    AsciiMessage getCommandString() noexcept override
    {
        std::snprintf(_commandString.begin(), _commandString.capacity(), "SIH");

        for (const auto& param : params)
        {
            // Print however many params exist
            std::snprintf(_commandString.end(), _commandString.capacity() - _commandString.length(), ",%+08.3f", param);
        }

        return _commandString;
    };
};

class AsyncOutputEnable : public Command
{
public:
    enum class State : uint8_t
    {
        Disable = 0,
        Enable = 1
    } state;

    AsyncOutputEnable(const State state) : Command{"ASY"}, state(state) {};

    AsciiMessage getCommandString() noexcept override
    {
        std::snprintf(_commandString.begin(), _commandString.capacity(), "ASY,%d", static_cast<uint8_t>(state));
        return _commandString;
    };
};

class SetFilterBias : public Command
{
public:
    SetFilterBias() : Command("SFB") {};
};

class PollBinaryOutputMessage : public Command
{
public:
    PollBinaryOutputMessage(const uint8_t binMsgNum) : Command{"BOM"}, binMsgNum(binMsgNum) {};
    uint8_t binMsgNum;
    AsciiMessage getCommandString() noexcept override
    {
        std::snprintf(_commandString.begin(), _commandString.capacity(), "BOM,%02d", binMsgNum);
        return _commandString;
    };
};

class SetBootLoader : public Command
{
public:
    enum class Processor : uint8_t
    {
        Nav = 0,
        Gnss = 1,
        Imu = 2,
        Poll = static_cast<uint8_t>('?')
    } processorId;

    SetBootLoader(const Processor processorId) : Command{"SBL"}, processorId(processorId) {};

    AsciiMessage getCommandString() noexcept override
    {
        if (processorId == Processor::Poll) { std::snprintf(_commandString.begin(), _commandString.capacity(), "SBL,?"); }
        else { std::snprintf(_commandString.begin(), _commandString.capacity(), "SBL,%d", static_cast<uint8_t>(processorId)); }
        return _commandString;
    }
    bool matchResponse(const AsciiMessage& responseToCheck, time_point timestamp) noexcept override
    {
        // Necessary because polling is an option, where we would like to populate the processorId with the response.
        if (Command::matchResponse(responseToCheck, timestamp))
        {
            if (!isMatchingError(_commandString))
            {
                auto u8_maybe = StringUtils::fromString<uint8_t>(_commandString.data() + 7, _commandString.data() + 8);
                if (!u8_maybe.has_value()) { return false; }             // This was invalid somehow.
                processorId = static_cast<Processor>(u8_maybe.value());  // SBL only has 1 digit processor ids
            }
            return true;
        }
        else { return false; }
    }
};

/// @brief This is the base class used by all register definitions. Its derived classes are directly used by the user.
class Register
{
public:
    Register(uint8_t id) : _id(id) {};

    virtual ~Register() {};

    /// @brief Generates a Read Register command from this object.
    virtual Command toReadCommand()
    {
        AsciiMessage responseMatch;

        std::snprintf(responseMatch.begin(), responseMatch.capacity(), "RRG,%02d", _id);

        return Command(responseMatch, (_id > 99) ? 7 : 6);
    }
    /// @brief Populates the contents of this register from a command.
    /// @return An error occurred.
    bool fromCommand(Command& commandIn) { return fromString(commandIn.getResponse()); }

    /// @brief Populates the contents of this register from a string.
    /// @return An error occurred.
    virtual bool fromString(const AsciiMessage& sensorResponse) = 0;

    /// @brief Formats the string payload necessary for a read or write register command.
    AsciiMessage toString() { return (_id < 10) ? "0" + std::to_string(_id) : std::to_string(_id); }

    /// @brief Gets the register id.
    uint8_t id() const { return _id; }

    /// @brief Gets the name of the register.
    virtual std::string name() = 0;

protected:
    uint8_t _id;
};

class MeasurementRegister : public Register
{
public:
    using Register::Register;

protected:
    using Register::_id;
};

/// @brief Base class inherited by all configuration registers.
class ConfigurationRegister : public Register
{
public:
    using Register::Register;

    virtual AsciiMessage toString() const = 0;

    /// @brief Generates a Write Register command from this object.
    Command toWriteCommand()
    {
        AsciiMessage responseMatch;
        std::snprintf(responseMatch.begin(), responseMatch.capacity(), "WRG,%02d,%s", _id, toString().c_str());
        return Command{responseMatch, static_cast<uint8_t>((_id > 99) ? 7 : 6)};
    }

protected:
    using Register::_id;
};

}  // namespace VN

#endif  // INTERFACE_COMMANDS_HPP
