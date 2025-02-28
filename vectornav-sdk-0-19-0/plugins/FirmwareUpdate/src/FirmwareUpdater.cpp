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

#include "FirmwareUpdater.hpp"

#include "HAL/Timer.hpp"
#include "HAL/Thread.hpp"
#include "Interface/Command.hpp"
#include "Interface/Sensor.hpp"
#include "Debug.hpp"

#include "Bootloader.hpp"

namespace VN
{
bool FirmwareUpdater::updateFirmware(Sensor* sensor, Filesystem::FilePath vnXmlPath, Params params)
{
    if (!Filesystem::exists(vnXmlPath))
    {
        std::cerr << "File does not exist: " << vnXmlPath << std::endl;
        return true;
    }

    InputFile vnXmlFile{true};
    if (vnXmlFile.open(vnXmlPath))
    {
        std::cerr << "Failed to open file: " << vnXmlPath << std::endl;
        return true;
    }
    if (updateFirmware(sensor, vnXmlFile, params)) { return true; }

    std::cout << "Firmware update complete." << std::endl;
    return false;
}

bool FirmwareUpdater::updateFirmware(Sensor* sensor, InputFile& vnXmlFile, Params params)
{
    if (!vnXmlFile.is_open())
    {
        std::cerr << "Firmware file is not open. Open before calling updateFirmware." << std::endl;
        return true;
    }
    VN::VnXml::Metadata vnXmlMetadata = VnXml::extractMetadata(vnXmlFile);

    _sensor = sensor;
    _navBaudRate = params.firmwareBaudRate;
    _bootloaderBaudRate = params.bootloaderBaudRate;
    // Reset the input stream to the beginning of the file
    vnXmlFile.reset();

    if (_tryUpdateNavFirmwareFromBootloader(vnXmlFile, vnXmlMetadata))
    {
        std::cout << "Failed to recover firmware." << std::endl;
        return true;
    }
    if (_pollSensorModelAndFirmwareVersion()) { return true; }
    std::cout << "Sensor model : " << _model << std::endl;
    Error latestError = _sensor->restoreFactorySettings();  // Has to happen before current processor polling because some firmware has a bug preventing a valid
                                                            // response outside of 115200
    if (latestError != Error::None) { std::cout << "Error " << latestError << " encountered when attempting to restore factory settings." << std::endl; }
    _navBaudRate = Sensor::BaudRate::Baud115200;
    if (_getCurrentProcessor())
    {
        std::cout << "Failed to get current processor, assuming Nav." << std::endl;
        _currentProcessor = Processor::Nav;
    }

    // Skip to the beginning of the processor's firmware data
    vnXmlFile.reset();
    AsciiMessage discard;
    size_t prevLineNum = 1;
    for (const auto& component : vnXmlMetadata)
    {
        // Skip to the beginning of the processor's firmware data
        const size_t numLinesToSkip = component.dataLineBegin - prevLineNum;
        for (size_t lineNo = 0; lineNo < numLinesToSkip; ++lineNo) { vnXmlFile.getLine(discard.begin(), discard.capacity()); }

        Sensor::BaudRate currBaudRate, currbootloaderBaudRate;
        if (VnXml::NavProcessorCheck::is_value(component.hardwareId))
        {
            if (_switchToNavProcessor()) { return true; }
            currBaudRate = _navBaudRate;
            currbootloaderBaudRate = _bootloaderBaudRate;
        }
        else if (VnXml::GnssProcessorCheck::is_value(component.hardwareId))
        {
            if (_switchToGnssProcessor()) { return true; }
            currBaudRate = _gnssBaudRate;
            currbootloaderBaudRate =
                static_cast<Sensor::BaudRate>(std::min(static_cast<uint32_t>(_bootloaderBaudRate), static_cast<uint32_t>(Sensor::BaudRate::Baud460800)));
            // SBL is not reliable over 460800
        }
        else if (VnXml::ImuProcessorCheck::is_value(component.hardwareId))
        {
            if (_switchToImuProcessor()) { return true; }
            currBaudRate = _imuBaudRate;
            currbootloaderBaudRate =
                static_cast<Sensor::BaudRate>(std::min(static_cast<uint32_t>(_bootloaderBaudRate), static_cast<uint32_t>(Sensor::BaudRate::Baud460800)));
            // SBL is not reliable over 460800
        }
        else { VN_ABORT(); }

        // Send records
        if (_updateProcessor(_sensor, vnXmlFile, currBaudRate, currbootloaderBaudRate, component.dataLineBegin,
                             component.dataLineEnd - component.dataLineBegin))
        {
            return true;
        }

        prevLineNum = component.dataLineEnd;  // Assume the bootloader consumed the whole file to set up for the next loop
    }
    if (_switchToNavProcessor()) { return true; }  // Finish by exiting to nav processor
    Error resetError = _sensor->reset();
    if (resetError != Error::None)
    {
        std::cout << resetError << " encountered when resetting the sensor." << std::endl;
        return true;
    }
    std::cout << "Firmware update complete." << std::endl;
    return false;
}

bool FirmwareUpdater::updateFirmware(Sensor* sensor, FilePaths files, Params params)
{
    for (const auto& currentFile : files)
    {
        if (!Filesystem::exists(currentFile.filePath))
        {
            std::cerr << "File does not exist: " << currentFile.filePath << std::endl;
            return true;
        }
        if (_tryOpenFile(currentFile.filePath)) { return true; }
        if (updateFirmware(sensor, _fileH, currentFile.processor, params)) { return true; }
        _fileH.close();
    }

    std::cout << "Firmware update complete." << std::endl;
    return false;
}

bool FirmwareUpdater::updateFirmware(Sensor* sensor, InputFile& vnxFile, const Processor processor, Params params)
{
    if (!vnxFile.is_open())
    {
        std::cerr << "Firmware file is not open. Update before calling updateFirmware" << std::endl;
        return true;
    }
    _sensor = sensor;
    _navBaudRate = params.firmwareBaudRate;
    _bootloaderBaudRate = params.bootloaderBaudRate;
    _totalLinesInFile = _calculateNumberOfLinesInFile(vnxFile);
    // Assume that we have a sensor in an invalid state due to a previously failed bootload attempt
    if (_tryUpdateNavFirmwareFromBootloader(vnxFile, processor))
    {
        std::cout << "Failed to recover firmware." << std::endl;
        return true;
    }
    if (_pollSensorModelAndFirmwareVersion()) { return true; }
    std::cout << "Sensor model : " << _model << std::endl;
    if (!_isCompatible(processor)) { return true; }
    Error latestError = _sensor->restoreFactorySettings();  // Has to happen before current processor polling because some firmware has a bug preventing a valid
                                                            // response outside of 115200
    _navBaudRate = Sensor::BaudRate::Baud115200;
    if (latestError != Error::None) { std::cout << "Error " << latestError << " encountered when attempting to restore factory settings." << std::endl; }
    if (_getCurrentProcessor())
    {
        std::cout << "Failed to get current processor, assuming Nav." << std::endl;
        _currentProcessor = Processor::Nav;
    }

    Sensor::BaudRate firmwareBaudRateToUse;
    Sensor::BaudRate bootloaderBaudRateToUse;
    switch (processor)
    {
        case (Processor::Nav):
        {
            if (_switchToNavProcessor()) { return true; }
            firmwareBaudRateToUse = _navBaudRate;
            bootloaderBaudRateToUse = _bootloaderBaudRate;
            break;
        }
        case (Processor::Imu):
        {
            if (_switchToImuProcessor()) { return true; }
            firmwareBaudRateToUse = _imuBaudRate;
            bootloaderBaudRateToUse =
                static_cast<Sensor::BaudRate>(std::min(static_cast<uint32_t>(_bootloaderBaudRate), static_cast<uint32_t>(Sensor::BaudRate::Baud460800)));
            // SBL is not reliable over 460800
            break;
        }
        case (Processor::Gnss):
        {
            if (_switchToGnssProcessor()) { return true; }
            firmwareBaudRateToUse = _gnssBaudRate;
            bootloaderBaudRateToUse =
                static_cast<Sensor::BaudRate>(std::min(static_cast<uint32_t>(_bootloaderBaudRate), static_cast<uint32_t>(Sensor::BaudRate::Baud460800)));
            // SBL is not reliable over 460800
            break;
        }
        default:
        {
            VN_ABORT();
        }
    }
    if (_updateProcessor(sensor, vnxFile, firmwareBaudRateToUse, bootloaderBaudRateToUse, 0, _totalLinesInFile)) { return true; }

    if (_switchToNavProcessor()) { return true; }  // Switch back to nav processor after the update in complete
    Error resetError = _sensor->reset();
    if (resetError != Error::None)
    {
        std::cout << resetError << " encountered when resetting the sensor." << std::endl;
        return true;
    }
    std::cout << "Firmware update complete." << std::endl;
    return false;
}

bool FirmwareUpdater::_tryUpdateNavFirmwareFromBootloader(InputFile& vnXmlFile, const VN::VnXml::Metadata& vnXmlMetadata)
{
    if (!Bootloader::autoconfigureBootloader(_sensor, Sensor::BaudRate::Baud115200))
    {  // If we are in recovery mode, don't try to go too fast.
        std::cout << "Attempting to recover firmware from corrupted file." << std::endl;
        // Bootloader detection succeeded.
        // We're in the bootloader and should just start by updating the nav firmware. Hope we're talking to nav.

        // Skip to the beginning of the processor's firmware data
        AsciiMessage discard;
        size_t prevLineNum = 1;
        for (const auto& component : vnXmlMetadata)
        {
            // Skip to the beginning of the processor's firmware data
            const size_t numLinesToSkip = component.dataLineBegin - prevLineNum;
            for (size_t lineNo = 0; lineNo < numLinesToSkip; ++lineNo) { vnXmlFile.getLine(discard.begin(), discard.capacity()); }
            if (!VnXml::NavProcessorCheck::is_value(component.hardwareId)) { continue; }
            if (_updateFirmware(_sensor, vnXmlFile, component.dataLineBegin, component.dataLineEnd - component.dataLineBegin)) { return true; }
            Bootloader::exitBootloader(_sensor);
            std::cout << "Recovered firmware version.\n";
            return false;  // We successfully updated the nav firmware.
        }
        // There wasn't a valid firmware file.
        _sensor->changeHostBaudRate(_navBaudRate);
        return true;
    }
    // If we couldn't confirm bootload, assume we didn't start in the bootloader and we can proceed as normal
    _sensor->changeHostBaudRate(_navBaudRate);
    return false;
}

bool FirmwareUpdater::_tryUpdateNavFirmwareFromBootloader(InputFile& vnxFile, Processor processor)
{
    if (!Bootloader::autoconfigureBootloader(_sensor, Sensor::BaudRate::Baud115200))
    {  // If we are in recovery mode, don't try to go too fast.
        std::cout << "Attempting to recover firmware from corrupted file." << std::endl;
        // Bootloader detection succeeded.
        // We're in the bootloader and should just start by updating the nav firmware. Hope we're talking to nav.
        if (processor != Processor::Nav) { return true; }
        if (_updateFirmware(_sensor, vnxFile, 0, _totalLinesInFile)) { return true; }
        Bootloader::exitBootloader(_sensor);
        std::cout << "Recovered firmware version.\n";
        return false;  // We successfully updated the nav firmware.
    }
    // If we couldn't confirm bootload, assume we didnt' start in the bootloader and we can proceed as normal
    return false;
}

bool FirmwareUpdater::_isCompatible(const Processor processor) const
{
    switch (processor)
    {
        case (Processor::Imu):
        {
            if (_model.at(4) == '0')
            {
                // Is industrial
                std::cout << "Error: Industrial sensors do not have an IMU processor.\n";
                return false;
            }
            break;
        }
        case (Processor::Gnss):
        {
            if (_model.at(3) != '3')
            {
                // Is not a VN-3X0
                std::cout << "Error: Only VN-3X0 have GNSS processors.\n";
                return false;
            }
            break;
        }
        case (Processor::Nav):
        {
            // All have nav, no failures here.
            break;
        }
        default:
            VN_ABORT();
    }
    return true;
}

bool FirmwareUpdater::_pollSensorModelAndFirmwareVersion()
{
    VN::Registers::System::Model modelRegister;
    Error latestError = _sensor->readRegister(&modelRegister);
    if (latestError == Error::ResponseTimeout)
    {
        // Try to recover by autoconnecting
        latestError = _sensor->autoConnect(_sensor->connectedPortName().value());
        if (latestError != Error::None)
        {
            std::cout << "Error " << latestError << " encountered when establishing communication." << std::endl;
            return true;
        }
        std::cout << "Warning: connected at " << _sensor->connectedBaudRate().value() << " bps." << std::endl;
        latestError = _sensor->readRegister(&modelRegister);
    }
    if (latestError != Error::None)
    {
        std::cout << "Error " << latestError << " encountered when reading model register." << std::endl;
        return true;
    }
    _model = modelRegister.model;

    VN::Registers::System::FwVer firmwareRegister;
    latestError = _sensor->readRegister(&firmwareRegister);
    if (latestError != Error::None)
    {
        std::cout << "Error " << latestError << " encountered when reading fimrware register." << std::endl;
        return true;
    }
    _firmwareVersion = firmwareRegister.fwVer;
    return false;
}

bool FirmwareUpdater::_updateProcessor(Sensor* sensor, InputFile& firmwareFile, const Sensor::BaudRate firmwareBaudRate,
                                       const Sensor::BaudRate bootloaderBaudRate, const size_t beginningLineNumber, const size_t numLinesInFirmware)
{
    VN::Registers::System::FwVer firmwareReg;
    Error latestError = sensor->readRegister(&firmwareReg);
    if (latestError != Error::None)
    {
        std::cout << "Error " << latestError << " encountered when reading firmware register." << std::endl;
        return true;
    }
    std::cout << "Initial firmware version: " << firmwareReg.fwVer << std::endl;

    // Entering Bootloader
    if (Bootloader::tryEnterBootloader(sensor, firmwareBaudRate, bootloaderBaudRate)) { return true; }

    // Flashing the .vnx file
    std::cout << "Updating processor.\n";
    if (_updateFirmware(sensor, firmwareFile, beginningLineNumber, numLinesInFirmware)) { return true; }
    Bootloader::exitBootloader(sensor);
    std::cout << "Processor updated.\n";

    latestError = sensor->readRegister(&firmwareReg);
    if (latestError != Error::None)
    {
        std::cout << "Error " << latestError << " encountered when reading firmware register." << std::endl;
        return true;
    }
    std::cout << "Updated firmware version: " << firmwareReg.fwVer << "\n" << std::endl;
    return false;
}

// Switch processors
bool FirmwareUpdater::_getCurrentProcessor()
{
    SetBootLoader sbl{SetBootLoader::Processor::Poll};
    Error latestError = _sensor->sendCommand(&sbl, Sensor::SendCommandBlockMode::BlockWithRetry);
    if (latestError != Error::None) { return true; }
    if (sbl.getError().has_value()) { return true; }
    _currentProcessor = sbl.processorId;

    return false;
}

bool FirmwareUpdater::_switchToNavProcessor()
{
    if (_currentProcessor == Processor::Nav) { return false; }
    auto splitModel = StringUtils::split(_model, '-');
    VN_ASSERT(splitModel.size() > 1);
    Error latestError = _sensor->setBootloader(Processor::Nav);
    if (latestError == Error::InvalidCommand && splitModel.at(1)[0] == '3')
    {  // Is VN-300
        DebugSwitches dbs{DebugSwitches::Switch::Processor, DebugSwitches::State::Main};
        latestError = _sensor->sendCommand(&dbs, Sensor::SendCommandBlockMode::Block, 6s);
        if (latestError != Error::None)
        {
            std::cout << "Error " << latestError << " encountered when switching to the Nav processor" << std::endl;
            return true;
        }
    }
    else if (latestError != Error::None)
    {
        std::cout << "Error " << latestError << " encountered when switching to the Nav processor." << std::endl;
        return true;
    }
    _sensor->changeHostBaudRate(_navBaudRate);
    thisThread::sleepFor(1s);
    if (!_sensor->verifySensorConnectivity())
    {
        latestError = _sensor->autoConnect(_sensor->connectedPortName().value());
        if (latestError != Error::None)
        {
            std::cout << "Error " << latestError << " encountered when autoconnecting." << std::endl;
            return true;
        }
        std::cout << "Warning: nav baud rate set at " << _sensor->connectedBaudRate().value() << std::endl;
    }

    _currentProcessor = Processor::Nav;
    std::cout << "Connected to Nav processor.\n";
    return false;
}

bool FirmwareUpdater::_switchToImuProcessor()
{
    if (_currentProcessor == Processor::Imu) { return false; }
    if (_switchToNavProcessor())
    {  // It's not always possible to switch directly between GNSS and IMU
        std::cout << "Failed to switch to Nav processor while switching to IMU processor" << std::endl;
        return true;
    }
    Error latestError = _sensor->setBootloader(Processor::Imu);
    if (latestError != Error::None)
    {
        std::cout << "Error " << latestError << " encountered when switching to the Imu processor." << std::endl;
        return true;
    }
    _sensor->changeHostBaudRate(_imuBaudRate);
    thisThread::sleepFor(1s);
    if (!_sensor->verifySensorConnectivity())
    {
        latestError = _sensor->autoConnect(_sensor->connectedPortName().value());
        if (latestError != Error::None)
        {
            std::cout << "Error " << latestError << " encountered when autoconnecting." << std::endl;
            return true;
        }
        std::cout << "Warning: IMU baud rate set at " << _sensor->connectedBaudRate().value() << std::endl;
    }

    _currentProcessor = Processor::Imu;
    std::cout << "Connected to IMU processor.\n";
    return false;
}

bool FirmwareUpdater::_switchToGnssProcessor()
{
    if (_currentProcessor == Processor::Gnss) { return false; }
    if (_switchToNavProcessor())
    {  // It's not always possible to switch directly between GNSS and IMU
        std::cout << "Failed to switch to Nav processor while switching to GNSS processor" << std::endl;
        return true;
    }

    auto splitModel = StringUtils::split(_model, '-');
    VN_ASSERT(splitModel.size() > 1);

    // Check family
    if (!(splitModel.at(1)[0] == '3') &&  // All 3XX products
        !(splitModel.at(1) == "210E"))
    {
        VN_ABORT();
    }

    Error latestError = _sensor->setBootloader(Processor::Gnss);
    if (latestError == Error::InvalidCommand)
    {
        // For VN-300 Legacy firmware 0.5.0.5 processor switch commands
        if (splitModel.at(1)[1] != '0')
        {  // Industrial
            std::cout << "Error " << latestError << " encountered when switching to the Gnss processor" << std::endl;
            return true;
        }

        if (splitModel.at(2)[0] == 'C')
        {  // Rugged
            DebugSwitches dbs{DebugSwitches::Switch::Processor, DebugSwitches::State::GPS};
            latestError = _sensor->sendCommand(&dbs, Sensor::SendCommandBlockMode::Block);
            if (latestError != Error::None)
            {
                std::cout << "Error " << latestError << " encountered when switching to the Gnss processor" << std::endl;
                return true;
            }
        }
        else
        {  // SMD
            ServicePortSwitch sps{};
            latestError = _sensor->sendCommand(&sps, Sensor::SendCommandBlockMode::Block);
            if (latestError != Error::None)
            {
                std::cout << "Error " << latestError << " encountered when switching to the Gnss processor" << std::endl;
                return true;
            }
            std::cout << "WARNING: A power cycle is required to connect to the Nav processor" << std::endl;
        }
    }
    else if (latestError != Error::None)
    {
        std::cout << "Error " << latestError << " encountered when switching to the Gnss processor" << std::endl;
        return true;
    }
    _sensor->changeHostBaudRate(_gnssBaudRate);
    thisThread::sleepFor(1s);
    if (!_sensor->verifySensorConnectivity())
    {
        latestError = _sensor->autoConnect(_sensor->connectedPortName().value());
        if (latestError != Error::None)
        {
            std::cout << "Error " << latestError << " encountered when autoconnecting." << std::endl;
            return true;
        }
        std::cout << "Warning: GNSS baud rate set at " << _sensor->connectedBaudRate().value() << std::endl;
    }

    _currentProcessor = Processor::Gnss;
    std::cout << "Connected to GNSS processor.\n";
    return false;
}

// Firmware update helpers
bool FirmwareUpdater::_updateFirmware(Sensor* sensor, InputFile& firmwareFile, const size_t lineNumberBeginning, const size_t numLinesInFirmware)
{
    const uint8_t numBootloaderTries = 2;
    for (uint8_t i = 0; i < numBootloaderTries; ++i)
    {
        Bootloader::FailureMode failure = Bootloader::sendRecords(sensor, firmwareFile, numLinesInFirmware);
        switch (failure)
        {
            case Bootloader::FailureMode::None:
            {
                return false;
            }
            case Bootloader::FailureMode::Retry:
            {
                firmwareFile.reset();
                for (size_t i = 0; i < lineNumberBeginning; ++i)
                {
                    AsciiMessage discard;
                    firmwareFile.getLine(discard.begin(), discard.capacity());
                }
                continue;
            }
            case Bootloader::FailureMode::Abort:
            {
                std::cerr << "Sending records failed.\n";
                return true;
            }
        }
    }
    return true;  // Max retires reached
}

bool FirmwareUpdater::_tryOpenFile(const Filesystem::FilePath& filePath)
{
    if (!Filesystem::exists(filePath))
    {
        std::cerr << "File does not exist: " << filePath << std::endl;
        return true;
    }
    if (_fileH.open(filePath))
    {
        std::cerr << "Error opening the file." << std::endl;
        _fileH.close();
        return true;
    }
    return false;
}

size_t FirmwareUpdater::_calculateNumberOfLinesInFile(InputFile& file) const
{
    size_t numberOfLines = 0;
    AsciiMessage line;
    while (!file.getLine(line.begin(), line.capacity())) { ++numberOfLines; }
    file.reset();
    return numberOfLines;
}

};  // namespace VN
