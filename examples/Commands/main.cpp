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

#include <iostream>

#include "Interface/Errors.hpp"
#include "Interface/Sensor.hpp"
#include "Interface/Commands.hpp"
#include "Interface/Registers.hpp"
#include "HAL/Thread.hpp"

using namespace VN;

std::string usage = "[port]\n";

int main(int argc, char* argv[])
{
    // This example demonstrates how to send commands without blocking, validating each command was received correctly
    const std::string portName = (argc > 1) ? argv[1] : "COM33";  // Change the sensor port name to the comm port of your local machine

    // Instantiate and connect to sensor
    Sensor sensor;
    Error latestError = sensor.autoConnect(portName);
    if (latestError != Error::None)
    {
        std::cout << "Error " << latestError << " encountered when connecting to " + portName << ".\t" << std::endl;
        return static_cast<int>(latestError);
    }
    std::cout << "Connected to " << portName << " at " << sensor.connectedBaudRate().value() << std::endl;

    // Read and print model number
    Registers::System::Model modelReg;
    latestError = sensor.readRegister(&modelReg);
    if (latestError != Error::None)
    {
        std::cout << "Error " << latestError << " encountered while reading from register " << modelReg.id() << " (" << modelReg.name() << ")" << std::endl;
        return static_cast<int>(latestError);
    }
    std::cout << "Model number: " << modelReg.model << std::endl;

    // Create and configure both a generic command and a Write Register command
    KnownMagneticDisturbance kmd(KnownMagneticDisturbance::State::Present);

    using VpeBasicControl = Registers::Attitude::VpeBasicControl;
    VpeBasicControl vpeBasicControl;
    vpeBasicControl.headingMode = VpeBasicControl::HeadingMode::Absolute;
    vpeBasicControl.filteringMode = VpeBasicControl::FilteringMode::AdaptivelyFiltered;
    vpeBasicControl.tuningMode = VpeBasicControl::TuningMode::Adaptive;
    Command vpeBasicControlWRG = vpeBasicControl.toWriteCommand();

    // Send our commands without blocking
    latestError = sensor.sendCommand(&kmd, Sensor::SendCommandBlockMode::None);
    if (latestError != Error::None)
    {
        std::cout << "Error " << latestError << " receieved while sending the Known Magnetic Disturbance Command: " << kmd.getCommandString() << std::endl;
        return static_cast<int>(latestError);
    }
    latestError = sensor.sendCommand(&vpeBasicControlWRG, Sensor::SendCommandBlockMode::None);
    if (latestError != Error::None)
    {
        std::cout << "Error " << latestError << " receieved while sending the following command: " << vpeBasicControlWRG.getCommandString() << std::endl;
        return static_cast<int>(latestError);
    }

    // ... Do some other application processes as we wait for a response
    thisThread::sleepFor(1s);

    // Check that there's a reponse, and print it
    if (kmd.getResponse().empty())
    {
        std::cout << "Error: KMD did not receive a valid response." << std::endl;
        return -1;
    }
    else
    {
        std::cout << "KMD response: " << kmd.getResponse() << std::endl;

        std::optional<Error> error_maybe = kmd.getError();
        if (error_maybe.has_value())
        {
            std::cout << "\tError: " << error_maybe.value() << std::endl;
            return static_cast<int>(error_maybe.value());
        }
    }

    if (vpeBasicControlWRG.getResponse().empty())
    {
        std::cout << "Error: WRG did not receive a valid response." << std::endl;
        return -1;
    }
    else
    {
        std::cout << "WRG response: " << vpeBasicControlWRG.getResponse() << std::endl;

        std::optional<Error> error_maybe = kmd.getError();
        if (error_maybe.has_value())
        {
            std::cout << "\tError: " << error_maybe.value() << std::endl;
            return static_cast<int>(error_maybe.value());
        }
    }

    sensor.disconnect();
    std::cout << "Commands example complete" << std::endl;
}
