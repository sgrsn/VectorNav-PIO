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

#include <cstdint>
#include <iostream>
#include <filesystem>

#include "Interface/Errors.hpp"
#include "Interface/Sensor.hpp"
#include "Interface/Registers.hpp"
#include "TemplateLibrary/ByteBuffer.hpp"

#include "SimpleLogger.hpp"

using namespace VN;
namespace fs = std::filesystem;

std::string usage = "[port] [path]\n";

int main(int argc, char* argv[])
{
    // Pass in port name and path as positional arguments, or edit them here
    const std::string portName = (argc > 1) ? argv[1] : "COM18";
    const std::string filePath = (argc > 2) ? argv[2] : (fs::path(__FILE__).parent_path() / "log.bin").string();

    Sensor sensor;

    Error latestError = sensor.autoConnect(portName);
    if (latestError != Error::None)
    {
        std::cout << "Error " << latestError << " encountered when connecting to " + portName << ".\t" << std::endl;
        return static_cast<int>(latestError);
    }
    std::cout << "Connected to " << portName << " at " << sensor.connectedBaudRate().value() << std::endl;

    Registers::System::Model modelRegister;

    latestError = sensor.readRegister(&modelRegister);
    if (latestError != Error::None)
    {
        std::cout << "Error" << latestError << " encountered when reading register " << modelRegister.id() << " (" << modelRegister.name() << ")" << std::endl;
        return static_cast<int>(latestError);
    }
    std::string modelNumber = modelRegister.model;
    std::cout << "Sensor Model Number: " << modelNumber << std::endl;

    // Set up resources needed for data logging
    ByteBuffer bufferToLog{1024 * 3};
    SimpleLogger logger{bufferToLog, filePath};

    // Register the logger's input buffer to receive all bytes from the sensor
    sensor.registerReceivedByteBuffer(&bufferToLog);

    // Create the log file and start logging to it
    if (logger.start())
    {
        std::cout << "Error: Failed to write to file." << std::endl;
        return 1;
    }

    std::cout << "Logging to " << filePath << std::endl;

    Timer timer{5s};
    timer.start();
    std::optional<AsyncError> asyncError;
    while (!timer.hasTimedOut())
    {
        std::this_thread::sleep_for(1ms);
        // Check to make sure we didn't get any async errors. Any buffer overruns means we have dropped data.
        asyncError = sensor.getAsynchronousError();
        if (asyncError) { std::cout << "Received async error: " << asyncError->error << std::endl; }
    }
    logger.stop();

    sensor.deregisterReceivedByteBuffer();
    sensor.disconnect();

    std::cout << "Logged " << std::to_string(logger.numBytesLogged()) << " bytes." << std::endl;

    std::cout << "SimpleLogger example complete.\n";
}
