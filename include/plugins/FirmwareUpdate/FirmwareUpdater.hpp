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

#ifndef FIRMWAREUPDATE_FIRMWAREUPDATER_HPP
#define FIRMWAREUPDATE_FIRMWAREUPDATER_HPP

#include <iostream>
#include <string>

#include "HAL/File.hpp"
#include "Interface/Command.hpp"
#include "Interface/Sensor.hpp"
#include "TemplateLibrary/String.hpp"
#include "VnXml.hpp"

namespace VN
{

/**
 * @class FirmwareUpdater
 * @brief Class responsible for updating firmware on a VectorNav unit.
 *
 * The FirmwareUpdater class provides functionality to update the firmware on a VectorNav unit. It handles all processor switching and supports updating
 * firmware from either a specified VNXML or a list of VNX files.
 */
class FirmwareUpdater
{
public:
    /**
     * @brief Alias for the processor type used in firmware updates.
     */
    using Processor = SetBootLoader::Processor;

    /**
     * @struct File
     * @brief Represents a VNX firmware file with its path and associated processor.
     */
    struct File
    {
        Filesystem::FilePath filePath;  ///< The path to the firmware file.
        Processor processor;            ///< The processor to which to upload the firmware.
    };

    /**
     * @struct Params
     * @brief Structure representing parameters for firmware and bootloader configurations.
     *
     * The Params struct encapsulates parameters related to firmware and bootloader configurations.
     * It includes default values for firmware and bootloader baud rate.
     */
    struct Params
    {
        Params() : firmwareBaudRate(Sensor::BaudRate::Baud115200), bootloaderBaudRate(Sensor::BaudRate::Baud115200) {}
        Params(Sensor::BaudRate firmwareBaudRate, Sensor::BaudRate bootloaderBaudRate)
            : firmwareBaudRate(firmwareBaudRate), bootloaderBaudRate(bootloaderBaudRate)
        {
        }
        Sensor::BaudRate firmwareBaudRate;    ///< The baud rate for firmware communication.
        Sensor::BaudRate bootloaderBaudRate;  ///< The baud rate for autobauded bootloader communication. This value is not recommended to surpass 460800 bps.
    };

    /**
     * @typedef FilePaths
     * @brief The firmware files to update the sensor, if using VNX files with manually-specified order.
     */
    using FilePaths = Vector<File, 6>;

    FirmwareUpdater() {}

    /**
     * @brief Update firmware on the sensor device from a VNXML file.
     *
     * This function updates the firmware on the specified unit using the specified VNXML file.
     *
     * @param sensor The sensor object which will receive the firmware updates.
     * @param vnXmlPath The path to the VNXML file containing firmware.
     * @param params Various firmware update parameters
     * @return Indicates if an error occurred. True if an error occurred, false if successful.
     */
    bool updateFirmware(Sensor* sensor, Filesystem::FilePath vnXmlPath, Params params = Params());

    /**
     * @brief Update firmware on the sensor device from a VNXML file object.
     *
     * This function updates the firmware on the specified unit using the provided VNXML file object.
     *
     * @param sensor The sensor object which will receive the firmware updates.
     * @param vnXmlFile  The file object for the VNXML file containing the firmware.
     * @param params Various firmware update parameters
     * @return Indicates if an error occurred. True if an error occurred, false if successful.
     */
    bool updateFirmware(Sensor* sensor, InputFile& vnXmlFile, Params params = Params());

    /**
     * @brief Update firmware on the sensor device from a list of firmware files.
     *
     * This function updates the firmware on the specified sensor device using a list of VNX files.
     *
     * @param sensor The sensor object which will receive the firmware updates.
     * @param files The list of firmware files and their associated processors. The firmwares will be updated as ordered by this parameter.
     * @param params Various firmware update parameters
     * @return Indicates if an error occured. True if an error occured, false if successful.
     */
    bool updateFirmware(Sensor* sensor, FilePaths files, Params params = Params());

    /**
     * @brief Update firmware on the sensor device from a VNX firmware file object.
     *
     * This function updates the firmware on the specified sensor device using a VNX file object.
     *
     * @param sensor The sensor object which will receive the firmware updates.
     * @param vnxFile The file object for the vnxFile containing the firmware.
     * @param processor The processor for the provided firmware file object.
     * @param params Various firmware update parameters.
     * @return Indicates if an error occured. True if an error occured, false if successful.
     */
    bool updateFirmware(Sensor* sensor, InputFile& vnxFile, const Processor processor, Params params = Params());

private:
    using ModelString = String<20>;

    bool _tryUpdateNavFirmwareFromBootloader(InputFile& vnXmlFile, const VN::VnXml::Metadata& vnXmlMetadata);
    bool _tryUpdateNavFirmwareFromBootloader(InputFile& vnxFile, Processor processor);

    bool _isCompatible(const Processor processor) const;

    bool _pollSensorModelAndFirmwareVersion();
    static bool _updateProcessor(Sensor* sensor, InputFile& firmwareFile, const Sensor::BaudRate firmwareBaudRate, const Sensor::BaudRate bootloaderBaudRate,
                                 const size_t beginningLineNumber, const size_t numLinesInFirmware);

    // Switch processors
    bool _getCurrentProcessor();

    bool _switchToNavProcessor();

    bool _switchToImuProcessor();

    bool _switchToGnssProcessor();

    // Firmware update helpers
    static bool _updateFirmware(Sensor* sensor, InputFile& firmwareFile, const size_t lineNumberBeginning, const size_t numLinesInFirmware);

    bool _tryOpenFile(const Filesystem::FilePath& filePath);

    size_t _calculateNumberOfLinesInFile(InputFile& filePath) const;

    InputFile _fileH{true};
    size_t _totalLinesInFile = 0;
    Sensor* _sensor;
    ModelString _model;
    std::string _firmwareVersion;
    Sensor::BaudRate _navBaudRate = Sensor::BaudRate::Baud115200;
    Sensor::BaudRate _imuBaudRate = Sensor::BaudRate::Baud115200;   // This baud rate should remain at 115200, because it is independent of the nav baud rate
    Sensor::BaudRate _gnssBaudRate = Sensor::BaudRate::Baud115200;  // This baud rate should remain at 115200, because it is independent of the nav baud rate
    Sensor::BaudRate _bootloaderBaudRate = Sensor::BaudRate::Baud115200;

    Processor _currentProcessor;
};

class DebugSwitches : public Command
{
public:
    enum class Switch : uint8_t
    {
        PowerToGPS = 1,
        Boot0State = 2,
        Processor = 3
    } debugSwitch;
    enum class State : uint8_t
    {
        Off = 0,
        On = 1,
        Main = 0,
        GPS = 1
    } state;  // BETTER WAY to HANDLE THIS??

    DebugSwitches(const Switch debugSwitch_, const State state_) : Command("DBS"), debugSwitch(debugSwitch_), state(state_) {};

    AsciiMessage getCommandString() noexcept override
    {
        std::snprintf(_commandString.begin(), _commandString.capacity(), "DBS,%d,%d", static_cast<uint8_t>(debugSwitch), static_cast<uint8_t>(state));
        return _commandString;
    }
};

class ServicePortSwitch : public Command
{
public:
    ServicePortSwitch() : Command("SPS") {}

    AsciiMessage getCommandString() noexcept override
    {
        std::snprintf(_commandString.begin(), _commandString.capacity(), "SPS,1,1,115200");
        return _commandString;
    }
};

class Microcontroller : public Command
{
public:
    enum class Processor : uint8_t
    {
        IMU = 1,
        CoProcessor = 2,
        GPS = 3
    } processor;
    Microcontroller(const Processor processor_) : Command("MCU"), processor(processor_) {};

    AsciiMessage getCommandString() noexcept override
    {
        std::snprintf(_commandString.begin(), _commandString.capacity(), "MCU,%d", static_cast<uint8_t>(processor));
        return _commandString;
    }
};

}  // namespace VN
#endif  // FIRMWAREUPDATE_FIRMWAREUPDATER_HPP
