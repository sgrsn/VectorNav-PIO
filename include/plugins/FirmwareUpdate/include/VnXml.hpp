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

#ifndef FIRMWAREUPDATE_VNXML_HPP
#define FIRMWAREUPDATE_VNXML_HPP

// #include <istream>
#include <cstdint>

#include "HAL/File.hpp"
#include "Interface/Errors.hpp"  // Necessary for EnumCheck

namespace VN
{

namespace VnXml
{

enum class MemoryType : uint8_t
{
    Firmware = 0,
    CalParams = 1,
    CalLog = 2,
    Serial = 3,
    Settings = 4,
    ReferenceModels = 5,
    GpsUpgrade = 6,
    Undefined = 255
};

enum class HardwareId : uint8_t
{
    VN100_v4 = 0,
    VN100_v4_CR = 1,
    VN100_v5 = 2,
    VN100_v5_CR = 3,
    VN100_v7 = 4,  // (also v6 with reduced ranges)
    VN200_v1 = 5,
    VN100_v7_USB = 6,
    VN200_HD_v1 = 7,
    VN300_IMU_v1 = 8,
    VN300_GPS_v1 = 9,
    VN300_IMU_v3 = 10,
    VN300_GPS_v3 = 11,
    VN150_v1 = 12,
    VN200_v3 = 13,
    VN150_v3 = 14,
    VNX10_NAV_v0 = 15,
    VNX10_IMU_v0 = 16,
    VNX10_GPS_v0 = 17,
    VNX10_COP_v0 = 18,
    VNX10_NAV_v1 = 19,
    VNX10_IMU_v1 = 20,
    VNX10_GPS_v1 = 21,
    VNX10_COP_v1 = 22,
    VNX10E_NAV_v1 = 23,
    VNX10E_IMU_v1 = 24,
    VNX10E_GPS_v1 = 25,
    VN361_v1 = 26,
    UserPrompt = 254,
    Undefined = 255,
};

using HardwareIdCheck =
    EnumCheck<HardwareId, HardwareId::VN100_v4, HardwareId::VN100_v4_CR, HardwareId::VN100_v5, HardwareId::VN100_v5_CR, HardwareId::VN100_v7,
              HardwareId::VN200_v1, HardwareId::VN100_v7_USB, HardwareId::VN200_HD_v1, HardwareId::VN300_IMU_v1, HardwareId::VN300_GPS_v1,
              HardwareId::VN300_IMU_v3, HardwareId::VN300_GPS_v3, HardwareId::VN150_v1, HardwareId::VN200_v3, HardwareId::VN150_v3, HardwareId::VNX10_NAV_v0,
              HardwareId::VNX10_IMU_v0, HardwareId::VNX10_GPS_v0, HardwareId::VNX10_COP_v0, HardwareId::VNX10_NAV_v1, HardwareId::VNX10_IMU_v1,
              HardwareId::VNX10_GPS_v1, HardwareId::VNX10_COP_v1, HardwareId::VNX10E_NAV_v1, HardwareId::VNX10E_IMU_v1, HardwareId::VNX10E_GPS_v1,
              HardwareId::VN361_v1, HardwareId::UserPrompt, HardwareId::Undefined>;

using MemoryTypeCheck = EnumCheck<MemoryType, MemoryType::Firmware, MemoryType::CalParams, MemoryType::CalLog, MemoryType::Serial, MemoryType::Settings,
                                  MemoryType::ReferenceModels, MemoryType::GpsUpgrade, MemoryType::Undefined>;

using NavProcessorCheck =
    EnumCheck<HardwareId, HardwareId::VN100_v4, HardwareId::VN100_v4_CR, HardwareId::VN100_v5, HardwareId::VN100_v5_CR, HardwareId::VN100_v7,
              HardwareId::VN200_v1, HardwareId::VN100_v7_USB, HardwareId::VN200_HD_v1, HardwareId::VN150_v1, HardwareId::VN200_v3, HardwareId::VN150_v3,
              HardwareId::VNX10_NAV_v0, HardwareId::VNX10_NAV_v1, HardwareId::VNX10E_NAV_v1, HardwareId::VN361_v1>;

using GnssProcessorCheck =
    EnumCheck<HardwareId, HardwareId::VN300_GPS_v1, HardwareId::VN300_GPS_v3, HardwareId::VNX10_GPS_v0, HardwareId::VNX10_GPS_v1, HardwareId::VNX10E_GPS_v1>;

using ImuProcessorCheck =
    EnumCheck<HardwareId, HardwareId::VN300_IMU_v1, HardwareId::VN300_IMU_v3, HardwareId::VNX10_IMU_v0, HardwareId::VNX10_IMU_v1, HardwareId::VNX10E_IMU_v1>;

struct Component
{
    MemoryType memoryType = MemoryType::Undefined;
    HardwareId hardwareId = HardwareId::Undefined;
    size_t dataLineBegin = 0;  // Defined as the first sendable data (not the xml data tag). 1-based.
    size_t dataLineEnd = 0;    // Defined as the last sendable data (not the xml data tag). 1-based.
};

constexpr uint8_t COMPONENT_CAPACITY = 5;
using Metadata = Vector<Component, COMPONENT_CAPACITY>;
Metadata extractMetadata(InputFile& vnXmlFile);
}  // namespace VnXml
}  // namespace VN
#endif  // FIRMWAREUPDATE_VNXML_HPP
