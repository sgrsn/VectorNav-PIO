# VectorNAV PlatformIO Library

This library enables easy integration of VectorNAV sensors with PlatformIO and Arduino-compatible platforms. 
## About
This library is based on the official VectorNAV SDK and has been adapted to work seamlessly with PlatformIO and Arduino-compatible platforms. It provides a C++ interface for communicating with VectorNAV sensors while maintaining compatibility with the original SDK's functionality.

## Installation

Add this library to your PlatformIO project's dependencies in your `platformio.ini` file:

```ini
lib_deps =
    https://github.com/sgrsn/VectorNav-PIO.git
```

## Getting Started

Include the necessary headers:

```cpp
#include "Interface/Sensor.hpp"
#include "Interface/Registers.hpp"
```

## Basic Usage

### Connecting to the Sensor

```cpp
// Define the serial port to use
std::string comPort = "Serial2";

// Create a sensor instance
VN::Sensor sensor;

// Connect to the sensor
VN::Error latestError = sensor.autoConnect(comPort);
```

### Reading Sensor Information

```cpp
// Read model information
VN::Registers::System::Model modelReg;
if (sensor.readRegister(&modelReg) == VN::Error::None) {
    std::cout << "Model Number: " << modelReg.model << std::endl;
}

// Read serial number
VN::Registers::System::Serial serNumReg;
if (sensor.readRegister(&serNumReg) == VN::Error::None) {
    std::cout << "Serial Number: " << serNumReg.serialNum << std::endl;
}
```

### Reading Sensor Measurements

```cpp
VN::Registers::IMU::Accel accelReg;
if (sensor.readRegister(&accelReg) == VN::Error::None)
{
    std::cout << "Accel: " << accelReg.accelX << ", " << accelReg.accelY << ", " << accelReg.accelZ << std::endl;
}
```

## Example

A complete example of connecting to a VectorNAV sensor, configuring binary output, and continuously reading accelerometer data:

```cpp
#include <Arduino.h>
#include "Interface/Sensor.hpp"
#include "Interface/Registers.hpp"

void setup() {
    Serial.begin(115200);
    std::cout << "Hello, VectorNav!" << std::endl;

    // Connect to the sensor on Serial2
    std::string comPort = "Serial2";
    VN::Sensor sensor;
    
    VN::Error latestError = sensor.autoConnect(comPort);
    if (latestError != VN::Error::None) {
        std::cout << "Error " << latestError << " occurred when autoconnecting." << std::endl;
        return;
    }
    
    // Configure binary output
    VN::Registers::System::BinaryOutput1 binaryOutput1Register;
    binaryOutput1Register.asyncMode = 3;
    binaryOutput1Register.rateDivisor = 40;
    binaryOutput1Register.common = 0x7FFF;
    sensor.writeRegister(&binaryOutput1Register);
    
    // Continuous reading loop
    while (true) {
        auto compositeData = sensor.getNextMeasurement();
        if (!compositeData) continue;
        
        if (compositeData->matchesMessage(binaryOutput1Register)) {
            VN::Vec3f accel = compositeData->imu.accel.value();
            std::cout << "Accel: " << accel[0] << ", " << accel[1] << ", " << accel[2] << std::endl;
        }
    }
}

void loop() {
    delay(1000);
}
```

## API Reference

## License

MIT license