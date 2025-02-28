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

#include <utility>
#include <iostream>

#include "pugixml/pugixml.hpp"
#include "Interface/Errors.hpp"
#include "RegisterScan.hpp"

namespace VN
{

std::array<std::pair<uint8_t, std::function<std::unique_ptr<VN::ConfigurationRegister>()>>, 37> RegScanFactory = {
    std::make_pair(0, []() { return std::make_unique<VN::Registers::ById::Reg0>(); }),
    {5, []() { return std::make_unique<VN::Registers::ById::Reg5>(); }},
    {6, []() { return std::make_unique<VN::Registers::ById::Reg6>(); }},
    {7, []() { return std::make_unique<VN::Registers::ById::Reg7>(); }},
    {21, []() { return std::make_unique<VN::Registers::ById::Reg21>(); }},
    {23, []() { return std::make_unique<VN::Registers::ById::Reg23>(); }},
    {25, []() { return std::make_unique<VN::Registers::ById::Reg25>(); }},
    {26, []() { return std::make_unique<VN::Registers::ById::Reg26>(); }},
    {30, []() { return std::make_unique<VN::Registers::ById::Reg30>(); }},
    {32, []() { return std::make_unique<VN::Registers::ById::Reg32>(); }},
    {35, []() { return std::make_unique<VN::Registers::ById::Reg35>(); }},
    {36, []() { return std::make_unique<VN::Registers::ById::Reg36>(); }},
    {38, []() { return std::make_unique<VN::Registers::ById::Reg38>(); }},
    {44, []() { return std::make_unique<VN::Registers::ById::Reg44>(); }},
    {50, []() { return std::make_unique<VN::Registers::ById::Reg50>(); }},
    {51, []() { return std::make_unique<VN::Registers::ById::Reg51>(); }},
    {55, []() { return std::make_unique<VN::Registers::ById::Reg55>(); }},
    {57, []() { return std::make_unique<VN::Registers::ById::Reg57>(); }},
    {67, []() { return std::make_unique<VN::Registers::ById::Reg67>(); }},
    {74, []() { return std::make_unique<VN::Registers::ById::Reg74>(); }},
    {75, []() { return std::make_unique<VN::Registers::ById::Reg75>(); }},
    {76, []() { return std::make_unique<VN::Registers::ById::Reg76>(); }},
    {77, []() { return std::make_unique<VN::Registers::ById::Reg77>(); }},
    {82, []() { return std::make_unique<VN::Registers::ById::Reg82>(); }},
    {83, []() { return std::make_unique<VN::Registers::ById::Reg83>(); }},
    {84, []() { return std::make_unique<VN::Registers::ById::Reg84>(); }},
    {85, []() { return std::make_unique<VN::Registers::ById::Reg85>(); }},
    {93, []() { return std::make_unique<VN::Registers::ById::Reg93>(); }},
    {99, []() { return std::make_unique<VN::Registers::ById::Reg99>(); }},
    {100, []() { return std::make_unique<VN::Registers::ById::Reg100>(); }},
    {101, []() { return std::make_unique<VN::Registers::ById::Reg101>(); }},
    {102, []() { return std::make_unique<VN::Registers::ById::Reg102>(); }},
    {105, []() { return std::make_unique<VN::Registers::ById::Reg105>(); }},
    {116, []() { return std::make_unique<VN::Registers::ById::Reg116>(); }},
    {144, []() { return std::make_unique<VN::Registers::ById::Reg144>(); }},
    {157, []() { return std::make_unique<VN::Registers::ById::Reg157>(); }},
    {206, []() { return std::make_unique<VN::Registers::ById::Reg206>(); }},
};

bool SensorConfigurator::ConfigureSensor(std::vector<std::unique_ptr<ConfigurationRegister>>& config)
{
    auto activePort = Registers::System::BaudRate::SerialPort::Serial1;

    for (const auto& reg : config)
    {
        std::cout << "Writing Register: " << std::to_string(reg->id()) << std::endl;
        Error error = Error::None;
        if (reg->id() == 5 && static_cast<Registers::System::BaudRate*>(reg.get())->serialPort == activePort)
        {
            error = sensor.changeBaudRate(static_cast<Registers::System::BaudRate*>(reg.get())->baudRate);
        }
        else { error = sensor.writeRegister(reg.get()); }
        if (error != Error::None) { return FAIL; }
    }

    return SUCCESS;
}

bool SensorConfigurator::SaveConfiguration(std::filesystem::path path)
{
    sensor.asyncOutputEnable(AsyncOutputEnable::State::Disable);

    Registers::System::Model modelReg;
    Registers::System::Serial serNumReg;
    Registers::System::FwVer fwVerReg;
    Registers::System::HwVer hwVerReg;

    if (sensor.readRegister(&modelReg) != Error::None || sensor.readRegister(&serNumReg) != Error::None || sensor.readRegister(&fwVerReg) != Error::None ||
        sensor.readRegister(&hwVerReg) != Error::None)
    {
        std::cout << "Failed to poll sensor information" << std::endl;
        return FAIL;
    }

    auto currentConfig = registerScan();

    if (sensor.restoreFactorySettings() != Error::None) { return FAIL; }

    sensor.asyncOutputEnable(AsyncOutputEnable::State::Disable);

    auto defaultConfig = registerScan();

    if (currentConfig.size() != defaultConfig.size())
    {
        // Something went wrong, so configure it to what it was and fail
        ConfigureSensor(currentConfig);
        VN_ASSERT(currentConfig.size() != defaultConfig.size());
    }

    std::vector<std::unique_ptr<ConfigurationRegister>> config;
    for (size_t i = 0; i < defaultConfig.size(); i++)
    {
        const auto id = defaultConfig[i]->id();
        const size_t configSizeInit = config.size();
        if (id == 5 || id == 6 || id == 7)
        {
            if (!(Registers::isConfigEqual(*defaultConfig[i], *currentConfig[i]) && Registers::isConfigEqual(*defaultConfig[i + 1], *currentConfig[i + 1])))
            {
                config.push_back(std::move(currentConfig[i]));
                config.push_back(std::move(currentConfig[i + 1]));
            }
            i++;
        }
        else if (id == 99)
        {
            auto reg = static_cast<Registers::GNSS::GnssSystemConfig*>(currentConfig[i].get());
            if (reg->receiverSelect == Registers::GNSS::GnssSystemConfig::ReceiverSelect::GnssAB)
            {
                if (!Registers::isConfigEqual(*defaultConfig[i], *currentConfig[i])) { config.push_back(std::move(currentConfig[i])); }
            }
            else
            {
                if (!(Registers::isConfigEqual(*defaultConfig[i], *currentConfig[i]) && Registers::isConfigEqual(*defaultConfig[i + 1], *currentConfig[i + 1])))
                {
                    config.push_back(std::move(currentConfig[i]));
                    config.push_back(std::move(currentConfig[i + 1]));
                }
                i++;
            }
        }
        else
        {
            if (!Registers::isConfigEqual(*defaultConfig[i], *currentConfig[i])) { config.push_back(std::move(currentConfig[i])); }
        }

        if (configSizeInit != config.size())
        {
            std::cout << "Reg " << std::to_string(id) << ": "
                      << "diff" << std::endl;
        }
        else
        {
            std::cout << "Reg " << std::to_string(id) << ": "
                      << "same" << std::endl;
        }
    }

    if (ConfigureSensor(config)) { return FAIL; }

    if (sensor.writeSettings() != Error::None) { std::cout << "Failed to write settings to non-volatile memory" << std::endl; }

    if (sensor.reset() != Error::None) { std::cout << "Failed to reset sensor. Static registers will not take affect until reset" << std::endl; }

    // If we made it to here. Everything is good
    pugi::xml_document doc;
    auto decl = doc.prepend_child(pugi::node_declaration);
    decl.append_attribute("version") = "1.0";
    decl.append_attribute("encoding") = "UTF-8";

    auto root = doc.append_child("VectorNav_Sensor");
    doc.document_element().append_attribute("xmlns:xsd") = "http://www.w3.org/2001/XMLSchema";
    doc.document_element().append_attribute("xmlns:xsi") = "http://www.w3.org/2001/XMLSchema-instance";
    root.append_child("Model").text().set(modelReg.model.c_str());
    root.append_child("HardwareRevision").text().set(std::to_string(hwVerReg.hwVer).c_str());
    root.append_child("SerialNumber").text().set(std::to_string(serNumReg.serialNum).c_str());
    root.append_child("FirmwareVersion").text().set(fwVerReg.fwVer.c_str());

    for (size_t i = 0; i < config.size(); i++)
    {
        auto reg = config[i].get();
        const auto id = reg->id();
        auto child = root.append_child("Register");
        child.append_child("Id").text().set(std::to_string(reg->id()).c_str());
        child.append_child("Name").text().set(reg->name().c_str());
        child.append_child("Values").text().set(reg->toString().c_str());
        if (id == 5 || id == 6 || id == 7)
        {
            auto reg2 = config[i + 1].get();
            child.child("Values").text().set((reg->toString() + ";" + reg2->toString()).c_str());
            i++;
        }
        else if (id == 99)
        {
            if ((i + 1 < config.size()) && (config[i + 1].get()->id() == 99))
            {
                auto reg2 = config[i + 1].get();
                child.child("Values").text().set((reg->toString() + ";" + reg2->toString()).c_str());
                i++;
            }
        }
    }

    doc.print(std::cout);
    doc.save_file(path.c_str());

    return SUCCESS;
}

bool SensorConfigurator::LoadConfiguration(std::filesystem::path path)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.c_str());

    if (!result)
    {
        std::cout << "file invalid" << std::endl;
        return FAIL;
    }

    std::cout << "xml file parsed" << std::endl;

    Registers::System::Model configModel;
    Registers::System::Serial configSerial;
    Registers::System::FwVer configFwVer;
    Registers::System::HwVer configHwVer;

    configModel.fromString(doc.child("VectorNav_Sensor").child("Model").first_child().value());
    configSerial.fromString(doc.child("VectorNav_Sensor").child("SerialNumber").first_child().value());
    configFwVer.fromString(doc.child("VectorNav_Sensor").child("FirmwareVersion").first_child().value());
    configHwVer.fromString(doc.child("VectorNav_Sensor").child("HardwareRevision").first_child().value());

    if (sensor.restoreFactorySettings() != Error::None) { return FAIL; }
    sensor.asyncOutputEnable(AsyncOutputEnable::State::Disable);

    std::vector<std::unique_ptr<ConfigurationRegister>> config;
    for (auto reg : doc.child("VectorNav_Sensor").children("Register"))
    {
        uint32_t regId = std::stoi(reg.child("Id").first_child().value());

        AsciiMessage values = reg.child("Values").first_child().value();

        auto params = StringUtils::split(values, ';');

        for (const auto& param : params)
        {
            auto reg = GetRegisterByIndex(regId);

            if (reg->fromString(param)) { VN_ABORT(); }

            config.push_back(std::move(reg));
        }
    }
    if (ConfigureSensor(config)) { return FAIL; }

    sensor.asyncOutputEnable(AsyncOutputEnable::State::Enable);

    return SUCCESS;
}

std::unique_ptr<VN::ConfigurationRegister> SensorConfigurator::GetRegisterByIndex(uint32_t idx)
{
    for (const auto& [regId, reg] : RegScanFactory)
    {
        if (regId == idx) { return reg(); }
    }
    return GetRegisterByIndex(0);
}

std::vector<std::unique_ptr<VN::ConfigurationRegister>> SensorConfigurator::registerScan()
{
    std::vector<std::unique_ptr<VN::ConfigurationRegister>> registers;

    for (const auto& [regId, factory] : RegScanFactory)
    {
        std::cout << "Polling register: " + std::to_string(regId) << std::endl;
        if ((regId == 5) || (regId == 6) || (regId == 7))
        {
            auto reg1 = factory();
            auto reg2 = factory();

            reg1->fromString("0,1");
            reg2->fromString("0,2");

            if (sensor.readRegister(reg1.get()) == Error::None && sensor.readRegister(reg2.get()) == Error::None)
            {
                registers.push_back(std::move(reg1));
                registers.push_back(std::move(reg2));
            }
        }
        else if (regId == 99)
        {
            using Registers::GNSS::GnssSystemConfig;
            auto reg1 = factory();
            auto reg2 = factory();

            static_cast<GnssSystemConfig*>(reg1.get())->receiverSelect = GnssSystemConfig::ReceiverSelect::GnssA;
            static_cast<GnssSystemConfig*>(reg2.get())->receiverSelect = GnssSystemConfig::ReceiverSelect::GnssB;

            if (sensor.readRegister(reg1.get()) == Error::None && sensor.readRegister(reg1.get()) == Error::None &&
                sensor.readRegister(reg2.get()) == Error::None && sensor.readRegister(reg2.get()) == Error::None)
            {
                registers.push_back(std::move(reg1));
                registers.push_back(std::move(reg2));
            }
            else
            {
                static_cast<GnssSystemConfig*>(reg1.get())->receiverSelect = GnssSystemConfig::ReceiverSelect::GnssAB;
                if (sensor.readRegister(reg1.get()) == Error::None && sensor.readRegister(reg1.get()) == Error::None) { registers.push_back(std::move(reg1)); }
            }
        }
        else
        {
            auto reg = factory();

            if (sensor.readRegister(reg.get()) == Error::None && sensor.readRegister(reg.get()) == Error::None) { registers.push_back(std::move(reg)); }
        }
    }

    return registers;
}
}  // namespace VN
