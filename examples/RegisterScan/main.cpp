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

#include "Interface/Sensor.hpp"
#include "Interface/Registers.hpp"
#include "RegisterScan.hpp"
#include <filesystem>
#include <string>

using namespace VN;

std::string usage = "[port] -s [fileLocation]\n[port] -l [fileLocation]\n";

int main([[maybe_unused]] int argc, char* argv[])
{
    // Parse command line arguments
    std::string comPort = argv[1];

    int loadOrSave = 0;
    if (std::strcmp("-s", argv[2]) == 0) { loadOrSave = 1; }
    else if (std::strcmp("-l", argv[2]) == 0) { loadOrSave = 2; }
    else
    {
        std::cout << "Error: Invalid usage.\n" << usage << std::endl;
        return -1;
    }

    std::filesystem::path path = std::string(argv[3]);

    // Connect to the sensor
    Sensor sensor;
    Error latestError = sensor.autoConnect(comPort);
    if (latestError != Error::None)
    {
        std::cout << "Error " << latestError << " occurred when autoconnecting." << std::endl;
        return 1;
    }
    std::cout << "Connected to " << comPort << " at " << sensor.connectedBaudRate().value() << std::endl;

    // Initialize Configuration object to perform RegisterScan
    SensorConfigurator configurator(sensor, comPort);

    // Run register scan
    bool failure;
    if (loadOrSave == 1) { failure = configurator.SaveConfiguration(path); }
    else { failure = configurator.LoadConfiguration(path); }

    // Disconnect from sensor
    sensor.disconnect();

    if (failure)
    {
        std::cout << "Error : Register Scan failed" << std::endl;
        return 1;
    }
    else
    {
        std::cout << "RegisterScan example complete" << std::endl;
        return 0;
    }
}
