#include <Arduino.h>

#include "Interface/Sensor.hpp"
#include "Interface/Registers.hpp"
#include "plugins/RegisterScan/RegisterScan.hpp"
#include <filesystem>
#include <string>
#include <sstream>

std::string comPort = "Serial2";

void setup()
{
    Serial.begin(115200);
    std::cout << "Hello, VectorNav!" << std::endl;

    // Connect to the sensor
    VN::Sensor sensor;
    // Parse command line arguments
    VN::Error latestError = sensor.autoConnect(comPort);
    if (latestError != VN::Error::None)
    {
        std::cout << "Error " << latestError << " occurred when autoconnecting." << std::endl;
        return 1;
    }
    std::cout << "Connected to " << comPort << " at " << sensor.connectedBaudRate().value() << std::endl;

    VN::Registers::System::Model modelReg;
    VN::Registers::System::Serial serNumReg;
    if( sensor.readRegister(&modelReg) == VN::Error::None )
    {
        std::cout << "Model Number: " << modelReg.model << std::endl;
    }
    
    if (sensor.readRegister(&serNumReg) == VN::Error::None )
    {
        std::cout << "Serial Number: " << serNumReg.serialNum << std::endl;
    }

    VN::Registers::System::BinaryOutput1 binaryOutput1Register;
    binaryOutput1Register.asyncMode = 3;
    binaryOutput1Register.rateDivisor = 40;
    binaryOutput1Register.common = 0x7FFF;
    if (sensor.writeRegister(&binaryOutput1Register) == VN::Error::None)
    {
        Serial.println("Binary Output 1 set");
    }
    
    while ( true )
    {
        VN::Sensor::CompositeDataQueueReturn compositeData = sensor.getNextMeasurement();
        // Check to make sure that a measurement is available
        if (!compositeData) continue;

        // Check to see if the measurement is a Binary Output 1 message
        if (compositeData->matchesMessage(binaryOutput1Register))
        {
            std::cout << "\tTime: " << compositeData->time.timeStartup.value().nanoseconds() << "\n";
            VN::Vec3f accel = compositeData->imu.accel.value();
            std::cout << "\tAccel X: " << accel[0] << "\n\tAccel Y: " << accel[1] << "\n\tAccel Z: " << accel[2] << "\n";
        }
    }
}

void loop()
{
    std::cout << "Please reset the sensor" << std::endl;
    delay(1000);
}