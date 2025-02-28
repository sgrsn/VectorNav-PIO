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

#include "Debug.hpp"
#include "Interface/Sensor.hpp"
#include "Interface/Command.hpp"
#include "Interface/Commands.hpp"
#include "Interface/Errors.hpp"

namespace VN
{

// ------------------------------------------
// Constructor and Desctructor
// ------------------------------------------

Sensor::Sensor()
{
    // Set up packet synchronizer
    _packetSynchronizer.addDispatcher(&_faPacketDispatcher);
    _packetSynchronizer.addDispatcher(&_asciiPacketDispatcher);
    _packetSynchronizer.addDispatcher(&_fbPacketDispatcher);
}

Sensor::~Sensor()
{
#if (THREADING_ENABLE)
    if (_listening)
    {  // Happens if exited by Error, and ensures that secondary thread closes out.
        _stopListening();
    }
#endif
}

// -------------------
// Serial Connectivity
// -------------------

Error Sensor::connect(const Serial_Base::PortName& portName, const BaudRate baudRate) noexcept
{
    Error lastError = _serial.open(portName, static_cast<uint32_t>(baudRate));
    if (lastError != Error::None) { return lastError; }
#if (THREADING_ENABLE)
    _startListening();
#endif
    return Error::None;
}

Error Sensor::autoConnect(const Serial_Base::PortName& portName) noexcept
{
    if constexpr (Config::CommandProcessor::commandProcQueueCapacity == 0) { return Error::CommandQueueFull; }
    std::array<BaudRate, 9> possibleBaudRates{
        BaudRate::Baud115200, BaudRate::Baud921600, BaudRate::Baud9600,   BaudRate::Baud19200,  BaudRate::Baud38400,
        BaudRate::Baud57600,  BaudRate::Baud128000, BaudRate::Baud230400, BaudRate::Baud460800,
    };
    Error error = connect(portName, BaudRate::Baud115200);
    if (error != Error::None) { return error; }
    for (const auto activeBaudRate : possibleBaudRates)
    {
        error = changeHostBaudRate(activeBaudRate);
        if (error == Error::UnsupportedBaudRate) { continue; }
        if (error != Error::None) { return error; }

        if (verifySensorConnectivity()) { return Error::None; }
    }
    disconnect();
    return Error::ResponseTimeout;
}

bool Sensor::verifySensorConnectivity() noexcept
{
    if constexpr (Config::CommandProcessor::commandProcQueueCapacity == 0) { return false; }
    Registers::System::Model modelRegister;
    modelRegister.model = "";

    Error readRegError = readRegister(&modelRegister, Config::Sensor::retryVerifyConnectivity);
    if (readRegError != Error::None) { return false; }
    return (modelRegister.model != "");
}

Error Sensor::changeBaudRate(const BaudRate newBaudRate) noexcept
{
    if constexpr (Config::CommandProcessor::commandProcQueueCapacity == 0) { return Error::CommandQueueFull; }
    if (!_serial.isSupportedBaudRate(static_cast<uint32_t>(newBaudRate))) { return Error::UnsupportedBaudRate; }

    Registers::System::BaudRate reg5;
    reg5.baudRate = newBaudRate;
    reg5.serialPort = Registers::System::BaudRate::SerialPort::ActiveSerial;
    Error latestError = writeRegister(&reg5, true);
    if (latestError != Error::None) { return latestError; }
    // Host baud rate may switch quicker than unit can switch its baud rate, so we need to give time to the unit before sending any subsequent verify calls
    thisThread::sleepFor(50ms);  // Appears that the sensor may take time to correctly configure, even if changing from and to the same baud rate. Tested by
                                 // changing baud rate in loop, and occasionally received invalid checksum.

    return changeHostBaudRate(newBaudRate);
}

Error Sensor::changeHostBaudRate(const BaudRate newBaudRate) noexcept
{
    if (connectedBaudRate().has_value() && newBaudRate == connectedBaudRate().value()) { return Error::None; }
#if (THREADING_ENABLE)
    _stopListening();
#endif
    Error lastError = _serial.changeBaudRate(static_cast<uint32_t>(newBaudRate));
    if (lastError != Error::None) { return lastError; }
#if (THREADING_ENABLE)
    _startListening();
#endif
    return Error::None;
}

void Sensor::disconnect() noexcept
{
#if (THREADING_ENABLE)
    _stopListening();
#endif
    _serial.close();
}

// ----------------------
// Accessing Measurements
// ----------------------

Sensor::CompositeDataQueueReturn Sensor::getNextMeasurement(const bool block) noexcept
{
    if constexpr (Config::PacketDispatchers::compositeDataQueueCapacity == 0) { return nullptr; }
    Timer timer(Config::Sensor::getMeasurementTimeoutLength);
    timer.start();
    CompositeDataQueueReturn queueReturn = _measurementQueue.get();
    if (!queueReturn)
    {
        if (block) { queueReturn = _blockOnMeasurement(timer, Config::Sensor::getMeasurementSleepDuration); }
    }
    return queueReturn;
}

Sensor::CompositeDataQueueReturn Sensor::getMostRecentMeasurement(const bool block) noexcept
{
    Timer timer(Config::Sensor::getMeasurementTimeoutLength);
    timer.start();
    CompositeDataQueueReturn queueReturn = _measurementQueue.getBack();
    if (!queueReturn)
    {
        if (block) { queueReturn = _blockOnMeasurement(timer, Config::Sensor::getMeasurementSleepDuration); }
    }
    return queueReturn;
}

Sensor::CompositeDataQueueReturn Sensor::_blockOnMeasurement(Timer& timer, [[maybe_unused]] const Microseconds sleepLength) noexcept
{
    bool hasTimedOut = false;
    bool retValHasValue = false;
    CompositeDataQueueReturn queueReturn;
    while (!retValHasValue && !hasTimedOut)
    {
#if (THREADING_ENABLE)
        thisThread::sleepFor(sleepLength);
#else
        bool needsMoreData = processNextPacket();
        if (needsMoreData)
        {
            Error lastError = loadMainBufferFromSerial();
            if (lastError != Error::None) { _asyncErrorQueue.put(AsyncError(lastError)); }
        }
#endif
        queueReturn = _measurementQueue.get();
        retValHasValue = queueReturn != nullptr;
        hasTimedOut = timer.hasTimedOut();
    }
    return queueReturn;
}

Error Sensor::_blockOnCommand(Command* command, Timer& timer) noexcept
{
    bool hasTimedOut = false;
#if (!THREADING_ENABLE)
    Error lastError = loadMainBufferFromSerial();
    if (lastError != Error::None) { _asyncErrorQueue.put(AsyncError(lastError)); }
#endif
    while (command->isAwaitingResponse())
    {
#if (THREADING_ENABLE)
        thisThread::sleepFor(Config::Sensor::commandSendSleepDuration);
#else
        bool needsMoreData = processNextPacket();
        if (needsMoreData)
        {
            thisThread::sleepFor(Config::Sensor::commandSendSleepDuration);
            lastError = loadMainBufferFromSerial();
            if (lastError != Error::None) { _asyncErrorQueue.put(AsyncError(lastError)); }
        }
#endif
        hasTimedOut = timer.hasTimedOut();
        if (hasTimedOut)
        {
            _commandProcessor.popCommandFromQueueBack();   // Since we're not tracking the command, let's remove it from the queue so that we can resend it
            command->matchResponse("FAIL", time_point());  // Ensure awaiting flag is set false
            VN_DEBUG_1("Command timed out.");
            return Error::ResponseTimeout;
        }
    };

    if (!command->hasValidResponse())
    {
        // The command was popped by another command's response
        VN_DEBUG_1("Command removed from queue.");
        return Error::ResponseTimeout;
    }

    auto maybeError = command->getError();
    if (maybeError.has_value()) { return maybeError.value(); }
    return Error::None;
}

// ----------------
// Sending Commands
// ----------------

Error Sensor::readRegister(Register* registerToRead, const bool retryOnFailure) noexcept
{
    if constexpr (Config::CommandProcessor::commandProcQueueCapacity == 0) { return Error::CommandQueueFull; }
    Command readCommand = registerToRead->toReadCommand();
    SendCommandBlockMode waitMode;
    if (retryOnFailure) { waitMode = SendCommandBlockMode::BlockWithRetry; }
    else { waitMode = SendCommandBlockMode::Block; }
    Error sendCommandError = sendCommand(&readCommand, waitMode);
    if (sendCommandError != Error::None) { return sendCommandError; }

    if (registerToRead->fromCommand(readCommand)) { return Error::ReceivedInvalidResponse; }
    return Error::None;
}

Error Sensor::writeRegister(ConfigurationRegister* registerToWrite, const bool retryOnFailure) noexcept
{
    if constexpr (Config::CommandProcessor::commandProcQueueCapacity == 0) { return Error::CommandQueueFull; }
    Command writeCommand = registerToWrite->toWriteCommand();
    SendCommandBlockMode waitMode;
    if (retryOnFailure) { waitMode = SendCommandBlockMode::BlockWithRetry; }
    else { waitMode = SendCommandBlockMode::Block; }
    const Error sendCommandError = sendCommand(&writeCommand, waitMode);
    if (sendCommandError != Error::None) { return sendCommandError; }
    if (registerToWrite->fromCommand(writeCommand)) { return Error::ReceivedInvalidResponse; };
    return Error::None;
}

Error Sensor::writeSettings() noexcept
{
    if constexpr (Config::CommandProcessor::commandProcQueueCapacity == 0) { return Error::CommandQueueFull; }
    WriteSettings wnv{};
    return sendCommand(&wnv, SendCommandBlockMode::Block, Config::Sensor::wnvSendTimeoutLength, Config::Sensor::wnvSendTimeoutLength * 2);
}

Error Sensor::reset() noexcept
{
    if constexpr (Config::CommandProcessor::commandProcQueueCapacity == 0) { return Error::CommandQueueFull; }
    Reset rst{};
    const Error sendCommandError = sendCommand(&rst, SendCommandBlockMode::BlockWithRetry);
    if (sendCommandError != Error::None) { return sendCommandError; }
    thisThread::sleepFor(Config::Sensor::resetSleepDuration);  // Give sensor time to start up
    if (!verifySensorConnectivity())
    {
        auto portName = _serial.connectedPortName();
        if (!portName.has_value()) { return Error::UnexpectedSerialError; }
        Error latestError = autoConnect(portName.value());
        if (latestError != Error::None) { return latestError; }
    }

    return Error::None;
}

Error Sensor::restoreFactorySettings() noexcept
{
    if constexpr (Config::CommandProcessor::commandProcQueueCapacity == 0) { return Error::CommandQueueFull; }
    RestoreFactorySettings rfs{};
    auto baudRate = _serial.connectedBaudRate();
    if (!baudRate) { return Error::SerialPortClosed; }

    Error sendCommandRetVal = sendCommand(&rfs, SendCommandBlockMode::Block, Config::Sensor::wnvSendTimeoutLength, Config::Sensor::wnvSendTimeoutLength * 2);
    if (*baudRate == 115200)
    {
        if (sendCommandRetVal != Error::None) { return sendCommandRetVal; }
    }
    else
    {
        // Allow the unit not to reply if not on default baud rate
        if (sendCommandRetVal != Error::None && sendCommandRetVal != Error::ResponseTimeout) { return sendCommandRetVal; }
    }
#if (THREADING_ENABLE)
    _stopListening();
#endif
    const Error changeBaudRateError = _serial.changeBaudRate(115200);
    if (changeBaudRateError != Error::None) { return changeBaudRateError; }
    thisThread::sleepFor(Config::Sensor::resetSleepDuration);  // Give sensor time to start up
#if (THREADING_ENABLE)
    _startListening();
#endif
    if (!verifySensorConnectivity()) { return Error::ResponseTimeout; }

    return Error::None;
}

Error Sensor::knownMagneticDisturbance(const KnownMagneticDisturbance::State state) noexcept
{
    KnownMagneticDisturbance kmd(state);
    return sendCommand(&kmd, SendCommandBlockMode::Block);
}

Error Sensor::knownAccelerationDisturbance(const KnownAccelerationDisturbance::State state) noexcept
{
    KnownAccelerationDisturbance kad(state);
    return sendCommand(&kad, SendCommandBlockMode::Block);
}

Error Sensor::setInitialHeading(const float heading) noexcept
{
    SetInitialHeading sih(heading);
    return sendCommand(&sih, SendCommandBlockMode::Block);
}

Error Sensor::setInitialHeading(const Ypr& ypr) noexcept
{
    SetInitialHeading sih(ypr);
    return sendCommand(&sih, SendCommandBlockMode::Block);
}

Error Sensor::setInitialHeading(const Quat& quat) noexcept
{
    SetInitialHeading sih(quat);
    return sendCommand(&sih, SendCommandBlockMode::Block);
}

Error Sensor::asyncOutputEnable(const AsyncOutputEnable::State state) noexcept
{
    AsyncOutputEnable asy(state);
    return sendCommand(&asy, SendCommandBlockMode::Block);
}

Error Sensor::setFilterBias() noexcept
{
    SetFilterBias sfb{};
    return sendCommand(&sfb, SendCommandBlockMode::Block);
}

Error Sensor::setBootloader(const SetBootLoader::Processor processorId) noexcept
{
    SetBootLoader sbl{processorId};
    return sendCommand(&sbl, SendCommandBlockMode::Block, 6s);
}

Error Sensor::sendCommand(Command* commandToSend, SendCommandBlockMode waitMode, const Microseconds waitLengthMs, const Microseconds timeoutThreshold) noexcept
{
    if constexpr (Config::CommandProcessor::commandProcQueueCapacity == 0) { return Error::CommandQueueFull; }
    CommandProcessor::RegisterCommandReturn regCommandReturn = _commandProcessor.registerCommand(commandToSend, timeoutThreshold);
    if (regCommandReturn.error != CommandProcessor::RegisterCommandReturn::Error::None)
    {
        if (regCommandReturn.error == CommandProcessor::RegisterCommandReturn::Error::CommandQueueFull) { return Error::CommandQueueFull; }
        else if (regCommandReturn.error == CommandProcessor::RegisterCommandReturn::Error::CommandResent) { return Error::CommandResent; }
        else { VN_ABORT(); }
    }
    Error lastError = _serial.send(regCommandReturn.message);
    if (lastError != Error::None) { return lastError; }

    if (waitMode == SendCommandBlockMode::None) { return Error::None; }
    Timer timer(waitLengthMs);
    timer.start();

    lastError = _blockOnCommand(commandToSend, timer);
    // We are attempting to try catch ResponseTimeout only
    if ((lastError == Error::None) || (lastError != Error::ResponseTimeout) || (waitMode != SendCommandBlockMode::BlockWithRetry)) { return lastError; }

    uint8_t retries = 0;  // Has already tried once
    while (!commandToSend->hasValidResponse())
    {  // Don't need timeout checked because it will error inside block
        timer.start();
        // We can resend because the command string will not have been overwriten by the response
        regCommandReturn = _commandProcessor.registerCommand(commandToSend);

        if (regCommandReturn.error != CommandProcessor::RegisterCommandReturn::Error::None)
        {
            if (regCommandReturn.error == CommandProcessor::RegisterCommandReturn::Error::CommandQueueFull) { return Error::CommandQueueFull; }
            else if (regCommandReturn.error == CommandProcessor::RegisterCommandReturn::Error::CommandResent) { return Error::CommandResent; }
            else { VN_ABORT(); }
        }

        lastError = _serial.send(regCommandReturn.message);
        if (lastError != Error::None) { return lastError; }

        lastError = _blockOnCommand(commandToSend, timer);
        if (lastError != Error::None)
        {
            if (lastError != Error::ResponseTimeout) { return lastError; }
            if (++retries == Config::Sensor::commandSendRetriesAllowed)
            {  // We have run out of retries
                return Error::ResponseTimeout;
            }
        }
    }
    return Error::None;
}

Error Sensor::serialSend(const AsciiMessage& msgToSend) noexcept
{
    Error lastError = _serial.send(msgToSend);
    if (lastError != Error::None) { return lastError; }
    return Error::None;
}

// ------------------
// Additional logging
// ------------------

Error Sensor::subscribeToMessage(PacketQueue_Interface* queueToSubscribe, const BinaryOutputMeasurements& binaryOutputMeasurmenetFilter,
                                 const FaSubscriberFilterType filterType) noexcept
{
    const bool failed = _faPacketDispatcher.addSubscriber(queueToSubscribe, binaryOutputMeasurmenetFilter.toBinaryHeader().toMeasurementHeader(), filterType);
    return failed ? Error::MessageSubscriberCapacityReached : Error::None;
}

Error Sensor::subscribeToMessage(PacketQueue_Interface* queueToSubscribe, const AsciiHeader& asciiHeaderFilter,
                                 const AsciiSubscriberFilterType filterType) noexcept
{
    const bool failed = _asciiPacketDispatcher.addSubscriber(queueToSubscribe, asciiHeaderFilter, filterType);
    return failed ? Error::MessageSubscriberCapacityReached : Error::None;
}

void Sensor::unsubscribeFromMessage(PacketQueue_Interface* queueToUnsubscribe, const SyncByte syncByte) noexcept
{
    switch (syncByte)
    {
        case (SyncByte::Ascii):
        {
            _asciiPacketDispatcher.removeSubscriber(queueToUnsubscribe);
            break;
        }
        case (SyncByte::FA):
        {
            _faPacketDispatcher.removeSubscriber(queueToUnsubscribe);
            break;
        }
        default:
            VN_ABORT();
    }
}

void Sensor::unsubscribeFromMessage(PacketQueue_Interface* queueToUnsubscribe, const BinaryOutputMeasurements& filter) noexcept
{
    _faPacketDispatcher.removeSubscriber(queueToUnsubscribe, filter.toBinaryHeader().toMeasurementHeader());
}

void Sensor::unsubscribeFromMessage(PacketQueue_Interface* queueToUnsubscribe, const AsciiHeader& filter) noexcept
{
    _asciiPacketDispatcher.removeSubscriber(queueToUnsubscribe, filter);
}

// ----------------------------
// Unthreaded Packet Processing
// ----------------------------

Error Sensor::loadMainBufferFromSerial() noexcept { return _serial.getData(); }

bool Sensor::processNextPacket() noexcept { return _packetSynchronizer.dispatchNextPacket(); }

#if (THREADING_ENABLE)

void Sensor::_listen() noexcept
{
    _mainByteBuffer.reset();
    while (_listening)
    {
        Error lastError = loadMainBufferFromSerial();
        if (lastError != Error::None) { _asyncErrorQueue.put(AsyncError(lastError)); }
        bool needsMoreData = false;
        while (!needsMoreData) { needsMoreData = processNextPacket(); }
        thisThread::sleepFor(Config::Sensor::listenSleepDuration);
    }
}

void Sensor::_startListening() noexcept
{
    if (_listening) { return; }
    _listening = true;
    _listeningThread = std::make_unique<Thread>(&Sensor::_listen, this);

    // _listeningThread->setHighestPriority(); // ** Commented as it is compile erroring or failing in runtime.
}

void Sensor::_stopListening() noexcept
{
    if (!_listening) { return; }
    _listening = false;
    _listeningThread->join();
}
#endif

// --------------
// Error Handling
// --------------

size_t Sensor::asynchronousErrorQueueSize() const noexcept { return _asyncErrorQueue.size(); }

std::optional<AsyncError> Sensor::getAsynchronousError() noexcept { return _asyncErrorQueue.get(); }

}  // namespace VN
