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

#include "HAL/Timer.hpp"
#include "Interface/Errors.hpp"
#include "Interface/Sensor.hpp"
#include "Interface/Registers.hpp"

using namespace VN;

std::string usage = "[port]\n";

int main(int argc, char* argv[])
{
    // This getting started example will walk you throgh the C++ usage of the SDK to connect to and interact with a VectorNav sensor.

    // This example will achieve the following:
    // 1. Connect to the sensor
    // 2. Poll and print the model number using a read register command
    // 3. Poll and print the current yaw, pitch, and roll using a read register command
    // 4. Configure the ADOR and ADOF to YPR at 2Hz
    // 5. Configure the first binary output to output timeStartup, accel, and angRate, all from common group, with a 200 rate divisor
    // 6. Enter a loop for 5 seconds where it:
    //    Determines which measurement it received (VNYPR or the necessary binary header)
    //    Prints out the relevant measurement from the CD struct
    // 7. Disconnect from sensor

    // Define the port connection parameters to be used later
    const std::string portName = (argc > 1) ? argv[1] : "COM33";  // Change the sensor port name to the comm port of your local machine

    // [1] Instantiate a sensor object we'll use to interact with the sensor, and connect to the unit
    Sensor sensor;
    Error latestError = sensor.autoConnect(portName);
    if (latestError != Error::None)
    {
        std::cout << "Error " << latestError << " encountered when connecting to " + portName << ".\t" << std::endl;
        return static_cast<int>(latestError);
    }
    std::cout << "Connected to " << portName << " at " << sensor.connectedBaudRate().value() << std::endl;

    // [2] Read and print the sensor model number
    // Create an empty register object of the necessary type, where the data member will be populated when the sensor responds to our "read register" request
    Registers::System::Model modelRegister;

    latestError = sensor.readRegister(&modelRegister);
    if (latestError != Error::None)
    {
        std::cout << "Error" << latestError << " encountered when reading register " << modelRegister.id() << " (" << modelRegister.name() << ")" << std::endl;
        return static_cast<int>(latestError);
    }
    std::string modelNumber = modelRegister.model;
    std::cout << "Sensor Model Number: " << modelNumber << std::endl;

    // [3] Read and print the current YPR
    Registers::Attitude::YawPitchRoll yprRegister;
    latestError = sensor.readRegister(&yprRegister);
    if (latestError != Error::None)
    {
        std::cout << "Error" << latestError << " encountered when reading register " << yprRegister.id() << " (" << yprRegister.name() << ")" << std::endl;
        return static_cast<int>(latestError);
    }
    std::cout << "Current Reading:  Yaw - " << yprRegister.yaw << " , Pitch - " << yprRegister.pitch << " , Roll - " << yprRegister.roll << std::endl;

    // [4] Configure the asynchronous ASCII output to YPR at 2Hz
    Registers::System::AsyncOutputType asyncDataOutputType;
    asyncDataOutputType.ador = Registers::System::AsyncOutputType::Ador::YPR;
    asyncDataOutputType.serialPort = Registers::System::AsyncOutputType::SerialPort::Serial1;
    latestError = sensor.writeRegister(&asyncDataOutputType);
    if (latestError != Error::None)
    {
        std::cout << "Error" << latestError << " encountered when configuring register " << asyncDataOutputType.id() << " (" << asyncDataOutputType.name()
                  << ")" << std::endl;
        return static_cast<int>(latestError);
    }
    else { std::cout << "ADOR configured\n"; }

    Registers::System::AsyncOutputFreq asyncDataOutputFrequency;
    asyncDataOutputFrequency.adof = Registers::System::AsyncOutputFreq::Adof::Rate1Hz;
    asyncDataOutputFrequency.serialPort = Registers::System::AsyncOutputFreq::SerialPort::Serial1;
    latestError = sensor.writeRegister(&asyncDataOutputFrequency);
    if (latestError != Error::None)
    {
        std::cout << "Error" << latestError << " encountered when configuring register " << asyncDataOutputFrequency.id() << " ("
                  << asyncDataOutputFrequency.name() << ")" << std::endl;
        return static_cast<int>(latestError);
    }
    else { std::cout << "ADOF configured\n"; }

    // [5] Configure the binary output to 2 Hz (1Hz if VN-300) through both serial ports, with specified outputs
    Registers::System::BinaryOutput1 binaryOutput1Register;
    binaryOutput1Register.rateDivisor = 400;
    binaryOutput1Register.asyncMode.serial1 = true;
    binaryOutput1Register.asyncMode.serial2 = true;
    binaryOutput1Register.common.timeStartup = true;
    binaryOutput1Register.common.accel = true;
    binaryOutput1Register.common.angularRate = true;
    binaryOutput1Register.common.imu = true;

    latestError = sensor.writeRegister(&binaryOutput1Register);
    if (latestError != Error::None)
    {
        std::cout << "Error" << latestError << " encountered when configuring register " << binaryOutput1Register.id() << " (" << binaryOutput1Register.name()
                  << ")" << std::endl;
        return static_cast<int>(latestError);
    }
    else { std::cout << "Binary output 1 message configured.\n"; }

    // [6] Listen to measurements from the sensor and print recognized packets
    Timer timer{5s};
    timer.start();
    while (!timer.hasTimedOut())
    {
        Sensor::CompositeDataQueueReturn compositeData = sensor.getNextMeasurement();
        // Check to make sure that a measurement is available
        if (!compositeData) continue;

        if (compositeData->matchesMessage(binaryOutput1Register))
        {
            std::cout << "Found binary 1 measurment.\n";

            std::cout << "\tTime: " << compositeData->time.timeStartup.value().nanoseconds() << "\n";
            Vec3f accel = compositeData->imu.accel.value();
            std::cout << "\tAccel X: " << accel[0] << "\n\tAccel Y: " << accel[1] << "\n\tAccel Z: " << accel[2] << "\n";
        }
        else if (compositeData->matchesMessage("VNYPR"))
        {
            std::cout << "Found Ascii ypr measurement.\n";

            Ypr ypr = compositeData->attitude.ypr.value();
            std::cout << "\tYaw: " << ypr.yaw << "\n\tPitch: " << ypr.pitch << "\n\tRoll: " << ypr.roll << std::endl;
        }
        else { std::cout << "Unrecognized asynchronous message received.\n"; }
    }

    // [7] Disconnect from the sensor
    sensor.disconnect();
    std::cout << "Sensor disconnected.\n";
    std::cout << "GettingStarted example complete." << std::endl;
}
