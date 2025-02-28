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


#include "Interface/Registers.hpp"
#include <cstdint>
#include "TemplateLibrary/String.hpp"

namespace VN
{
    namespace Registers
    {
        Vector<const char*, Config::PacketFinders::asciiMaxFieldCount> findIndexOfFieldSeparators(const AsciiMessage& input);
        namespace Attitude
        {
            bool YawPitchRoll::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 3 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto yaw_tmp = StringUtils::fromString<float>(start, end);
                if(!yaw_tmp.has_value()) { return true; }
                yaw = yaw_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto pitch_tmp = StringUtils::fromString<float>(start, end);
                if(!pitch_tmp.has_value()) { return true; }
                pitch = pitch_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto roll_tmp = StringUtils::fromString<float>(start, end);
                if(!roll_tmp.has_value()) { return true; }
                roll = roll_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const YawPitchRoll& lhs, const YawPitchRoll& rhs)
            {
                return 
                (
                (lhs.yaw == rhs.yaw)
                && (lhs.pitch == rhs.pitch)
                && (lhs.roll == rhs.roll)
                );
            }
            
            bool Quaternion::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 4 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto quatX_tmp = StringUtils::fromString<float>(start, end);
                if(!quatX_tmp.has_value()) { return true; }
                quatX = quatX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto quatY_tmp = StringUtils::fromString<float>(start, end);
                if(!quatY_tmp.has_value()) { return true; }
                quatY = quatY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto quatZ_tmp = StringUtils::fromString<float>(start, end);
                if(!quatZ_tmp.has_value()) { return true; }
                quatZ = quatZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto quatS_tmp = StringUtils::fromString<float>(start, end);
                if(!quatS_tmp.has_value()) { return true; }
                quatS = quatS_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const Quaternion& lhs, const Quaternion& rhs)
            {
                return 
                (
                (lhs.quatX == rhs.quatX)
                && (lhs.quatY == rhs.quatY)
                && (lhs.quatZ == rhs.quatZ)
                && (lhs.quatS == rhs.quatS)
                );
            }
            
            bool QuatMagAccelRate::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 13 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto quatX_tmp = StringUtils::fromString<float>(start, end);
                if(!quatX_tmp.has_value()) { return true; }
                quatX = quatX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto quatY_tmp = StringUtils::fromString<float>(start, end);
                if(!quatY_tmp.has_value()) { return true; }
                quatY = quatY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto quatZ_tmp = StringUtils::fromString<float>(start, end);
                if(!quatZ_tmp.has_value()) { return true; }
                quatZ = quatZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto quatS_tmp = StringUtils::fromString<float>(start, end);
                if(!quatS_tmp.has_value()) { return true; }
                quatS = quatS_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magX_tmp = StringUtils::fromString<float>(start, end);
                if(!magX_tmp.has_value()) { return true; }
                magX = magX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magY_tmp = StringUtils::fromString<float>(start, end);
                if(!magY_tmp.has_value()) { return true; }
                magY = magY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magZ_tmp = StringUtils::fromString<float>(start, end);
                if(!magZ_tmp.has_value()) { return true; }
                magZ = magZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelX_tmp = StringUtils::fromString<float>(start, end);
                if(!accelX_tmp.has_value()) { return true; }
                accelX = accelX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelY_tmp = StringUtils::fromString<float>(start, end);
                if(!accelY_tmp.has_value()) { return true; }
                accelY = accelY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelZ_tmp = StringUtils::fromString<float>(start, end);
                if(!accelZ_tmp.has_value()) { return true; }
                accelZ = accelZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroX_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroX_tmp.has_value()) { return true; }
                gyroX = gyroX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroY_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroY_tmp.has_value()) { return true; }
                gyroY = gyroY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroZ_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroZ_tmp.has_value()) { return true; }
                gyroZ = gyroZ_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const QuatMagAccelRate& lhs, const QuatMagAccelRate& rhs)
            {
                return 
                (
                (lhs.quatX == rhs.quatX)
                && (lhs.quatY == rhs.quatY)
                && (lhs.quatZ == rhs.quatZ)
                && (lhs.quatS == rhs.quatS)
                && (lhs.magX == rhs.magX)
                && (lhs.magY == rhs.magY)
                && (lhs.magZ == rhs.magZ)
                && (lhs.accelX == rhs.accelX)
                && (lhs.accelY == rhs.accelY)
                && (lhs.accelZ == rhs.accelZ)
                && (lhs.gyroX == rhs.gyroX)
                && (lhs.gyroY == rhs.gyroY)
                && (lhs.gyroZ == rhs.gyroZ)
                );
            }
            
            bool MagGravRefVec::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 6 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magRefN_tmp = StringUtils::fromString<float>(start, end);
                if(!magRefN_tmp.has_value()) { return true; }
                magRefN = magRefN_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magRefE_tmp = StringUtils::fromString<float>(start, end);
                if(!magRefE_tmp.has_value()) { return true; }
                magRefE = magRefE_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magRefD_tmp = StringUtils::fromString<float>(start, end);
                if(!magRefD_tmp.has_value()) { return true; }
                magRefD = magRefD_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gravRefN_tmp = StringUtils::fromString<float>(start, end);
                if(!gravRefN_tmp.has_value()) { return true; }
                gravRefN = gravRefN_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gravRefE_tmp = StringUtils::fromString<float>(start, end);
                if(!gravRefE_tmp.has_value()) { return true; }
                gravRefE = gravRefE_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gravRefD_tmp = StringUtils::fromString<float>(start, end);
                if(!gravRefD_tmp.has_value()) { return true; }
                gravRefD = gravRefD_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage MagGravRefVec::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%f,%f,%f,%f,%f,%f", magRefN,magRefE,magRefD,gravRefN,gravRefE,gravRefD);
                return result;
            }
            
            bool operator==(const MagGravRefVec& lhs, const MagGravRefVec& rhs)
            {
                return 
                (
                (lhs.magRefN == rhs.magRefN)
                && (lhs.magRefE == rhs.magRefE)
                && (lhs.magRefD == rhs.magRefD)
                && (lhs.gravRefN == rhs.gravRefN)
                && (lhs.gravRefE == rhs.gravRefE)
                && (lhs.gravRefD == rhs.gravRefD)
                );
            }
            
            bool YprMagAccelAngularRates::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 12 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto yaw_tmp = StringUtils::fromString<float>(start, end);
                if(!yaw_tmp.has_value()) { return true; }
                yaw = yaw_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto pitch_tmp = StringUtils::fromString<float>(start, end);
                if(!pitch_tmp.has_value()) { return true; }
                pitch = pitch_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto roll_tmp = StringUtils::fromString<float>(start, end);
                if(!roll_tmp.has_value()) { return true; }
                roll = roll_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magX_tmp = StringUtils::fromString<float>(start, end);
                if(!magX_tmp.has_value()) { return true; }
                magX = magX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magY_tmp = StringUtils::fromString<float>(start, end);
                if(!magY_tmp.has_value()) { return true; }
                magY = magY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magZ_tmp = StringUtils::fromString<float>(start, end);
                if(!magZ_tmp.has_value()) { return true; }
                magZ = magZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelX_tmp = StringUtils::fromString<float>(start, end);
                if(!accelX_tmp.has_value()) { return true; }
                accelX = accelX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelY_tmp = StringUtils::fromString<float>(start, end);
                if(!accelY_tmp.has_value()) { return true; }
                accelY = accelY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelZ_tmp = StringUtils::fromString<float>(start, end);
                if(!accelZ_tmp.has_value()) { return true; }
                accelZ = accelZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroX_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroX_tmp.has_value()) { return true; }
                gyroX = gyroX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroY_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroY_tmp.has_value()) { return true; }
                gyroY = gyroY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroZ_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroZ_tmp.has_value()) { return true; }
                gyroZ = gyroZ_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const YprMagAccelAngularRates& lhs, const YprMagAccelAngularRates& rhs)
            {
                return 
                (
                (lhs.yaw == rhs.yaw)
                && (lhs.pitch == rhs.pitch)
                && (lhs.roll == rhs.roll)
                && (lhs.magX == rhs.magX)
                && (lhs.magY == rhs.magY)
                && (lhs.magZ == rhs.magZ)
                && (lhs.accelX == rhs.accelX)
                && (lhs.accelY == rhs.accelY)
                && (lhs.accelZ == rhs.accelZ)
                && (lhs.gyroX == rhs.gyroX)
                && (lhs.gyroY == rhs.gyroY)
                && (lhs.gyroZ == rhs.gyroZ)
                );
            }
            
            bool VpeBasicControl::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 4 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto resv_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!resv_tmp.has_value()) { return true; }
                resv = resv_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto headingMode_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!headingMode_tmp.has_value()) { return true; }
                headingMode = static_cast<VpeBasicControl::HeadingMode>(headingMode_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto filteringMode_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!filteringMode_tmp.has_value()) { return true; }
                filteringMode = static_cast<VpeBasicControl::FilteringMode>(filteringMode_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto tuningMode_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!tuningMode_tmp.has_value()) { return true; }
                tuningMode = static_cast<VpeBasicControl::TuningMode>(tuningMode_tmp.value());
                
                return false; // no errors occurred
            }
            
            AsciiMessage VpeBasicControl::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%u,%u,%u,%u", resv,static_cast<uint8_t>(headingMode),static_cast<uint8_t>(filteringMode),static_cast<uint8_t>(tuningMode));
                return result;
            }
            
            bool operator==(const VpeBasicControl& lhs, const VpeBasicControl& rhs)
            {
                return 
                (
                (lhs.resv == rhs.resv)
                && (lhs.headingMode == rhs.headingMode)
                && (lhs.filteringMode == rhs.filteringMode)
                && (lhs.tuningMode == rhs.tuningMode)
                );
            }
            
            bool VpeMagBasicTuning::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 9 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto baseTuningX_tmp = StringUtils::fromString<float>(start, end);
                if(!baseTuningX_tmp.has_value()) { return true; }
                baseTuningX = baseTuningX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto baseTuningY_tmp = StringUtils::fromString<float>(start, end);
                if(!baseTuningY_tmp.has_value()) { return true; }
                baseTuningY = baseTuningY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto baseTuningZ_tmp = StringUtils::fromString<float>(start, end);
                if(!baseTuningZ_tmp.has_value()) { return true; }
                baseTuningZ = baseTuningZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto adaptiveTuningX_tmp = StringUtils::fromString<float>(start, end);
                if(!adaptiveTuningX_tmp.has_value()) { return true; }
                adaptiveTuningX = adaptiveTuningX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto adaptiveTuningY_tmp = StringUtils::fromString<float>(start, end);
                if(!adaptiveTuningY_tmp.has_value()) { return true; }
                adaptiveTuningY = adaptiveTuningY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto adaptiveTuningZ_tmp = StringUtils::fromString<float>(start, end);
                if(!adaptiveTuningZ_tmp.has_value()) { return true; }
                adaptiveTuningZ = adaptiveTuningZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto adaptiveFilteringX_tmp = StringUtils::fromString<float>(start, end);
                if(!adaptiveFilteringX_tmp.has_value()) { return true; }
                adaptiveFilteringX = adaptiveFilteringX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto adaptiveFilteringY_tmp = StringUtils::fromString<float>(start, end);
                if(!adaptiveFilteringY_tmp.has_value()) { return true; }
                adaptiveFilteringY = adaptiveFilteringY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto adaptiveFilteringZ_tmp = StringUtils::fromString<float>(start, end);
                if(!adaptiveFilteringZ_tmp.has_value()) { return true; }
                adaptiveFilteringZ = adaptiveFilteringZ_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage VpeMagBasicTuning::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%f,%f,%f,%f,%f,%f,%f,%f,%f", baseTuningX,baseTuningY,baseTuningZ,adaptiveTuningX,adaptiveTuningY,adaptiveTuningZ,adaptiveFilteringX,adaptiveFilteringY,adaptiveFilteringZ);
                return result;
            }
            
            bool operator==(const VpeMagBasicTuning& lhs, const VpeMagBasicTuning& rhs)
            {
                return 
                (
                (lhs.baseTuningX == rhs.baseTuningX)
                && (lhs.baseTuningY == rhs.baseTuningY)
                && (lhs.baseTuningZ == rhs.baseTuningZ)
                && (lhs.adaptiveTuningX == rhs.adaptiveTuningX)
                && (lhs.adaptiveTuningY == rhs.adaptiveTuningY)
                && (lhs.adaptiveTuningZ == rhs.adaptiveTuningZ)
                && (lhs.adaptiveFilteringX == rhs.adaptiveFilteringX)
                && (lhs.adaptiveFilteringY == rhs.adaptiveFilteringY)
                && (lhs.adaptiveFilteringZ == rhs.adaptiveFilteringZ)
                );
            }
            
            bool VpeAccelBasicTuning::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 9 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto baseTuningX_tmp = StringUtils::fromString<float>(start, end);
                if(!baseTuningX_tmp.has_value()) { return true; }
                baseTuningX = baseTuningX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto baseTuningY_tmp = StringUtils::fromString<float>(start, end);
                if(!baseTuningY_tmp.has_value()) { return true; }
                baseTuningY = baseTuningY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto baseTuningZ_tmp = StringUtils::fromString<float>(start, end);
                if(!baseTuningZ_tmp.has_value()) { return true; }
                baseTuningZ = baseTuningZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto adaptiveTuningX_tmp = StringUtils::fromString<float>(start, end);
                if(!adaptiveTuningX_tmp.has_value()) { return true; }
                adaptiveTuningX = adaptiveTuningX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto adaptiveTuningY_tmp = StringUtils::fromString<float>(start, end);
                if(!adaptiveTuningY_tmp.has_value()) { return true; }
                adaptiveTuningY = adaptiveTuningY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto adaptiveTuningZ_tmp = StringUtils::fromString<float>(start, end);
                if(!adaptiveTuningZ_tmp.has_value()) { return true; }
                adaptiveTuningZ = adaptiveTuningZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto adaptiveFilteringX_tmp = StringUtils::fromString<float>(start, end);
                if(!adaptiveFilteringX_tmp.has_value()) { return true; }
                adaptiveFilteringX = adaptiveFilteringX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto adaptiveFilteringY_tmp = StringUtils::fromString<float>(start, end);
                if(!adaptiveFilteringY_tmp.has_value()) { return true; }
                adaptiveFilteringY = adaptiveFilteringY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto adaptiveFilteringZ_tmp = StringUtils::fromString<float>(start, end);
                if(!adaptiveFilteringZ_tmp.has_value()) { return true; }
                adaptiveFilteringZ = adaptiveFilteringZ_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage VpeAccelBasicTuning::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%f,%f,%f,%f,%f,%f,%f,%f,%f", baseTuningX,baseTuningY,baseTuningZ,adaptiveTuningX,adaptiveTuningY,adaptiveTuningZ,adaptiveFilteringX,adaptiveFilteringY,adaptiveFilteringZ);
                return result;
            }
            
            bool operator==(const VpeAccelBasicTuning& lhs, const VpeAccelBasicTuning& rhs)
            {
                return 
                (
                (lhs.baseTuningX == rhs.baseTuningX)
                && (lhs.baseTuningY == rhs.baseTuningY)
                && (lhs.baseTuningZ == rhs.baseTuningZ)
                && (lhs.adaptiveTuningX == rhs.adaptiveTuningX)
                && (lhs.adaptiveTuningY == rhs.adaptiveTuningY)
                && (lhs.adaptiveTuningZ == rhs.adaptiveTuningZ)
                && (lhs.adaptiveFilteringX == rhs.adaptiveFilteringX)
                && (lhs.adaptiveFilteringY == rhs.adaptiveFilteringY)
                && (lhs.adaptiveFilteringZ == rhs.adaptiveFilteringZ)
                );
            }
            
            bool YprLinearBodyAccelAngularRates::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 9 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto yaw_tmp = StringUtils::fromString<float>(start, end);
                if(!yaw_tmp.has_value()) { return true; }
                yaw = yaw_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto pitch_tmp = StringUtils::fromString<float>(start, end);
                if(!pitch_tmp.has_value()) { return true; }
                pitch = pitch_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto roll_tmp = StringUtils::fromString<float>(start, end);
                if(!roll_tmp.has_value()) { return true; }
                roll = roll_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto linAccelX_tmp = StringUtils::fromString<float>(start, end);
                if(!linAccelX_tmp.has_value()) { return true; }
                linAccelX = linAccelX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto linAccelY_tmp = StringUtils::fromString<float>(start, end);
                if(!linAccelY_tmp.has_value()) { return true; }
                linAccelY = linAccelY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto linAccelZ_tmp = StringUtils::fromString<float>(start, end);
                if(!linAccelZ_tmp.has_value()) { return true; }
                linAccelZ = linAccelZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroX_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroX_tmp.has_value()) { return true; }
                gyroX = gyroX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroY_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroY_tmp.has_value()) { return true; }
                gyroY = gyroY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroZ_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroZ_tmp.has_value()) { return true; }
                gyroZ = gyroZ_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const YprLinearBodyAccelAngularRates& lhs, const YprLinearBodyAccelAngularRates& rhs)
            {
                return 
                (
                (lhs.yaw == rhs.yaw)
                && (lhs.pitch == rhs.pitch)
                && (lhs.roll == rhs.roll)
                && (lhs.linAccelX == rhs.linAccelX)
                && (lhs.linAccelY == rhs.linAccelY)
                && (lhs.linAccelZ == rhs.linAccelZ)
                && (lhs.gyroX == rhs.gyroX)
                && (lhs.gyroY == rhs.gyroY)
                && (lhs.gyroZ == rhs.gyroZ)
                );
            }
            
            bool YprLinearInertialAccelAngularRates::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 9 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto yaw_tmp = StringUtils::fromString<float>(start, end);
                if(!yaw_tmp.has_value()) { return true; }
                yaw = yaw_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto pitch_tmp = StringUtils::fromString<float>(start, end);
                if(!pitch_tmp.has_value()) { return true; }
                pitch = pitch_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto roll_tmp = StringUtils::fromString<float>(start, end);
                if(!roll_tmp.has_value()) { return true; }
                roll = roll_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto linAccelN_tmp = StringUtils::fromString<float>(start, end);
                if(!linAccelN_tmp.has_value()) { return true; }
                linAccelN = linAccelN_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto linAccelE_tmp = StringUtils::fromString<float>(start, end);
                if(!linAccelE_tmp.has_value()) { return true; }
                linAccelE = linAccelE_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto linAccelD_tmp = StringUtils::fromString<float>(start, end);
                if(!linAccelD_tmp.has_value()) { return true; }
                linAccelD = linAccelD_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroX_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroX_tmp.has_value()) { return true; }
                gyroX = gyroX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroY_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroY_tmp.has_value()) { return true; }
                gyroY = gyroY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroZ_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroZ_tmp.has_value()) { return true; }
                gyroZ = gyroZ_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const YprLinearInertialAccelAngularRates& lhs, const YprLinearInertialAccelAngularRates& rhs)
            {
                return 
                (
                (lhs.yaw == rhs.yaw)
                && (lhs.pitch == rhs.pitch)
                && (lhs.roll == rhs.roll)
                && (lhs.linAccelN == rhs.linAccelN)
                && (lhs.linAccelE == rhs.linAccelE)
                && (lhs.linAccelD == rhs.linAccelD)
                && (lhs.gyroX == rhs.gyroX)
                && (lhs.gyroY == rhs.gyroY)
                && (lhs.gyroZ == rhs.gyroZ)
                );
            }
            
        } // namespace Attitude
        
        namespace GNSS
        {
            bool GnssBasicConfig::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 5 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto receiverEnable_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!receiverEnable_tmp.has_value()) { return true; }
                receiverEnable = static_cast<GnssBasicConfig::ReceiverEnable>(receiverEnable_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto ppsSource_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!ppsSource_tmp.has_value()) { return true; }
                ppsSource = static_cast<GnssBasicConfig::PpsSource>(ppsSource_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto rate_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!rate_tmp.has_value()) { return true; }
                rate = static_cast<GnssBasicConfig::Rate>(rate_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto resv4_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!resv4_tmp.has_value()) { return true; }
                resv4 = resv4_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto antPower_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!antPower_tmp.has_value()) { return true; }
                antPower = static_cast<GnssBasicConfig::AntPower>(antPower_tmp.value());
                
                return false; // no errors occurred
            }
            
            AsciiMessage GnssBasicConfig::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%u,%u,%u,%u,%u", static_cast<uint8_t>(receiverEnable),static_cast<uint8_t>(ppsSource),static_cast<uint8_t>(rate),resv4,static_cast<uint8_t>(antPower));
                return result;
            }
            
            bool operator==(const GnssBasicConfig& lhs, const GnssBasicConfig& rhs)
            {
                return 
                (
                (lhs.receiverEnable == rhs.receiverEnable)
                && (lhs.ppsSource == rhs.ppsSource)
                && (lhs.rate == rhs.rate)
                && (lhs.resv4 == rhs.resv4)
                && (lhs.antPower == rhs.antPower)
                );
            }
            
            bool GnssAOffset::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 3 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto positionX_tmp = StringUtils::fromString<float>(start, end);
                if(!positionX_tmp.has_value()) { return true; }
                positionX = positionX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto positionY_tmp = StringUtils::fromString<float>(start, end);
                if(!positionY_tmp.has_value()) { return true; }
                positionY = positionY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto positionZ_tmp = StringUtils::fromString<float>(start, end);
                if(!positionZ_tmp.has_value()) { return true; }
                positionZ = positionZ_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage GnssAOffset::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%f,%f,%f", positionX,positionY,positionZ);
                return result;
            }
            
            bool operator==(const GnssAOffset& lhs, const GnssAOffset& rhs)
            {
                return 
                (
                (lhs.positionX == rhs.positionX)
                && (lhs.positionY == rhs.positionY)
                && (lhs.positionZ == rhs.positionZ)
                );
            }
            
            bool GnssSolLla::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 15 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gps1Tow_tmp = StringUtils::fromString<double>(start, end);
                if(!gps1Tow_tmp.has_value()) { return true; }
                gps1Tow = gps1Tow_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gps1Week_tmp = StringUtils::fromString<uint16_t>(start, end);
                if(!gps1Week_tmp.has_value()) { return true; }
                gps1Week = gps1Week_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gnss1Fix_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!gnss1Fix_tmp.has_value()) { return true; }
                gnss1Fix = static_cast<GnssSolLla::Gnss1Fix>(gnss1Fix_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gnss1NumSats_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!gnss1NumSats_tmp.has_value()) { return true; }
                gnss1NumSats = gnss1NumSats_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto lat_tmp = StringUtils::fromString<double>(start, end);
                if(!lat_tmp.has_value()) { return true; }
                lat = lat_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto lon_tmp = StringUtils::fromString<double>(start, end);
                if(!lon_tmp.has_value()) { return true; }
                lon = lon_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto alt_tmp = StringUtils::fromString<double>(start, end);
                if(!alt_tmp.has_value()) { return true; }
                alt = alt_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velN_tmp = StringUtils::fromString<float>(start, end);
                if(!velN_tmp.has_value()) { return true; }
                velN = velN_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velE_tmp = StringUtils::fromString<float>(start, end);
                if(!velE_tmp.has_value()) { return true; }
                velE = velE_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velD_tmp = StringUtils::fromString<float>(start, end);
                if(!velD_tmp.has_value()) { return true; }
                velD = velD_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posUncertaintyN_tmp = StringUtils::fromString<float>(start, end);
                if(!posUncertaintyN_tmp.has_value()) { return true; }
                posUncertaintyN = posUncertaintyN_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posUncertaintyE_tmp = StringUtils::fromString<float>(start, end);
                if(!posUncertaintyE_tmp.has_value()) { return true; }
                posUncertaintyE = posUncertaintyE_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posUncertaintyD_tmp = StringUtils::fromString<float>(start, end);
                if(!posUncertaintyD_tmp.has_value()) { return true; }
                posUncertaintyD = posUncertaintyD_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gnss1VelUncertainty_tmp = StringUtils::fromString<float>(start, end);
                if(!gnss1VelUncertainty_tmp.has_value()) { return true; }
                gnss1VelUncertainty = gnss1VelUncertainty_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gnss1TimeUncertainty_tmp = StringUtils::fromString<float>(start, end);
                if(!gnss1TimeUncertainty_tmp.has_value()) { return true; }
                gnss1TimeUncertainty = gnss1TimeUncertainty_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const GnssSolLla& lhs, const GnssSolLla& rhs)
            {
                return 
                (
                (lhs.gps1Tow == rhs.gps1Tow)
                && (lhs.gps1Week == rhs.gps1Week)
                && (lhs.gnss1Fix == rhs.gnss1Fix)
                && (lhs.gnss1NumSats == rhs.gnss1NumSats)
                && (lhs.lat == rhs.lat)
                && (lhs.lon == rhs.lon)
                && (lhs.alt == rhs.alt)
                && (lhs.velN == rhs.velN)
                && (lhs.velE == rhs.velE)
                && (lhs.velD == rhs.velD)
                && (lhs.posUncertaintyN == rhs.posUncertaintyN)
                && (lhs.posUncertaintyE == rhs.posUncertaintyE)
                && (lhs.posUncertaintyD == rhs.posUncertaintyD)
                && (lhs.gnss1VelUncertainty == rhs.gnss1VelUncertainty)
                && (lhs.gnss1TimeUncertainty == rhs.gnss1TimeUncertainty)
                );
            }
            
            bool GnssSolEcef::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 15 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gps1Tow_tmp = StringUtils::fromString<double>(start, end);
                if(!gps1Tow_tmp.has_value()) { return true; }
                gps1Tow = gps1Tow_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gps1Week_tmp = StringUtils::fromString<uint16_t>(start, end);
                if(!gps1Week_tmp.has_value()) { return true; }
                gps1Week = gps1Week_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gnss1Fix_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!gnss1Fix_tmp.has_value()) { return true; }
                gnss1Fix = static_cast<GnssSolEcef::Gnss1Fix>(gnss1Fix_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gnss1NumSats_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!gnss1NumSats_tmp.has_value()) { return true; }
                gnss1NumSats = gnss1NumSats_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posX_tmp = StringUtils::fromString<double>(start, end);
                if(!posX_tmp.has_value()) { return true; }
                posX = posX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posY_tmp = StringUtils::fromString<double>(start, end);
                if(!posY_tmp.has_value()) { return true; }
                posY = posY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posZ_tmp = StringUtils::fromString<double>(start, end);
                if(!posZ_tmp.has_value()) { return true; }
                posZ = posZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velX_tmp = StringUtils::fromString<float>(start, end);
                if(!velX_tmp.has_value()) { return true; }
                velX = velX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velY_tmp = StringUtils::fromString<float>(start, end);
                if(!velY_tmp.has_value()) { return true; }
                velY = velY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velZ_tmp = StringUtils::fromString<float>(start, end);
                if(!velZ_tmp.has_value()) { return true; }
                velZ = velZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posUncertaintyX_tmp = StringUtils::fromString<float>(start, end);
                if(!posUncertaintyX_tmp.has_value()) { return true; }
                posUncertaintyX = posUncertaintyX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posUncertaintyY_tmp = StringUtils::fromString<float>(start, end);
                if(!posUncertaintyY_tmp.has_value()) { return true; }
                posUncertaintyY = posUncertaintyY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posUncertaintyZ_tmp = StringUtils::fromString<float>(start, end);
                if(!posUncertaintyZ_tmp.has_value()) { return true; }
                posUncertaintyZ = posUncertaintyZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gnss1VelUncertainty_tmp = StringUtils::fromString<float>(start, end);
                if(!gnss1VelUncertainty_tmp.has_value()) { return true; }
                gnss1VelUncertainty = gnss1VelUncertainty_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gnss1TimeUncertainty_tmp = StringUtils::fromString<float>(start, end);
                if(!gnss1TimeUncertainty_tmp.has_value()) { return true; }
                gnss1TimeUncertainty = gnss1TimeUncertainty_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const GnssSolEcef& lhs, const GnssSolEcef& rhs)
            {
                return 
                (
                (lhs.gps1Tow == rhs.gps1Tow)
                && (lhs.gps1Week == rhs.gps1Week)
                && (lhs.gnss1Fix == rhs.gnss1Fix)
                && (lhs.gnss1NumSats == rhs.gnss1NumSats)
                && (lhs.posX == rhs.posX)
                && (lhs.posY == rhs.posY)
                && (lhs.posZ == rhs.posZ)
                && (lhs.velX == rhs.velX)
                && (lhs.velY == rhs.velY)
                && (lhs.velZ == rhs.velZ)
                && (lhs.posUncertaintyX == rhs.posUncertaintyX)
                && (lhs.posUncertaintyY == rhs.posUncertaintyY)
                && (lhs.posUncertaintyZ == rhs.posUncertaintyZ)
                && (lhs.gnss1VelUncertainty == rhs.gnss1VelUncertainty)
                && (lhs.gnss1TimeUncertainty == rhs.gnss1TimeUncertainty)
                );
            }
            
            bool GnssSystemConfig::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 9 + 1)
                {
                    // This register may not have an optional field
                    if (tokens.size() != 8 + 1)
                    {
                        return true;
                    }
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto systems_tmp = StringUtils::fromStringHex<uint16_t>(start, end);
                if(!systems_tmp.has_value()) { return true; }
                systems = systems_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto minCno_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!minCno_tmp.has_value()) { return true; }
                minCno = minCno_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto minElev_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!minElev_tmp.has_value()) { return true; }
                minElev = minElev_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto maxSats_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!maxSats_tmp.has_value()) { return true; }
                maxSats = maxSats_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto sbasMode_tmp = StringUtils::fromStringHex<uint8_t>(start, end);
                if(!sbasMode_tmp.has_value()) { return true; }
                sbasMode = sbasMode_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto sbasSelect1_tmp = StringUtils::fromStringHex<uint16_t>(start, end);
                if(!sbasSelect1_tmp.has_value()) { return true; }
                sbasSelect1 = sbasSelect1_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto sbasSelect2_tmp = StringUtils::fromStringHex<uint16_t>(start, end);
                if(!sbasSelect2_tmp.has_value()) { return true; }
                sbasSelect2 = sbasSelect2_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto sbasSelect3_tmp = StringUtils::fromStringHex<uint16_t>(start, end);
                if(!sbasSelect3_tmp.has_value()) { return true; }
                sbasSelect3 = sbasSelect3_tmp.value();
                
                if(tokens.size() == (9 + 1))
                {
                    // This is an optional parameter.
                    start = tokens[index++] + 1;
                    end = tokens[index];
                    const auto receiverSelect_tmp = StringUtils::fromString<uint8_t>(start, end);
                    if(!receiverSelect_tmp.has_value()) { return true; }
                    receiverSelect = static_cast<GnssSystemConfig::ReceiverSelect>(receiverSelect_tmp.value());
                }
                
                return false; // no errors occurred
            }
            
            Command GnssSystemConfig::toReadCommand()
            {
                AsciiMessage commandString;
                if (receiverSelect == GNSS::GnssSystemConfig::ReceiverSelect::GnssAB)
                {
                    std::snprintf(commandString.begin(), commandString.capacity(), "RRG,%02d", _id);
                }
                else
                {
                    std::snprintf(commandString.begin(), commandString.capacity(), "RRG,%02d,%1d", _id, static_cast<uint8_t>(receiverSelect));
                }
                Command readCommand(commandString, 6);
                return readCommand;
            }
            
            
            AsciiMessage GnssSystemConfig::toString() const
            {
                AsciiMessage result = "";
                if (receiverSelect == GNSS::GnssSystemConfig::ReceiverSelect::GnssAB)
                {
                    std::snprintf(result.begin(), result.capacity(), "%04X,%d,%d,%d,%02X,%04X,%04X,%04X", uint16_t(systems),minCno,minElev,maxSats,uint8_t(sbasMode),uint16_t(sbasSelect1),uint16_t(sbasSelect2),uint16_t(sbasSelect3));
                }
                else
                {
                    std::snprintf(result.begin(), result.capacity(), "%04X,%d,%d,%d,%02X,%04X,%04X,%04X,%d", uint16_t(systems),minCno,minElev,maxSats,uint8_t(sbasMode),uint16_t(sbasSelect1),uint16_t(sbasSelect2),uint16_t(sbasSelect3),static_cast<uint8_t>(receiverSelect));
                }
                return result;
            }
            
            bool operator==(const GnssSystemConfig& lhs, const GnssSystemConfig& rhs)
            {
                return 
                (
                (uint16_t(lhs.systems) == uint16_t(rhs.systems))
                && (lhs.minCno == rhs.minCno)
                && (lhs.minElev == rhs.minElev)
                && (lhs.maxSats == rhs.maxSats)
                && (uint8_t(lhs.sbasMode) == uint8_t(rhs.sbasMode))
                && (uint16_t(lhs.sbasSelect1) == uint16_t(rhs.sbasSelect1))
                && (uint16_t(lhs.sbasSelect2) == uint16_t(rhs.sbasSelect2))
                && (uint16_t(lhs.sbasSelect3) == uint16_t(rhs.sbasSelect3))
                && (lhs.receiverSelect == rhs.receiverSelect)
                );
            }
            
            bool GnssSyncConfig::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 7 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gnssSyncEnable_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!gnssSyncEnable_tmp.has_value()) { return true; }
                gnssSyncEnable = static_cast<GnssSyncConfig::GnssSyncEnable>(gnssSyncEnable_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto polarity_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!polarity_tmp.has_value()) { return true; }
                polarity = static_cast<GnssSyncConfig::Polarity>(polarity_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto specType_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!specType_tmp.has_value()) { return true; }
                specType = static_cast<GnssSyncConfig::SpecType>(specType_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto resv_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!resv_tmp.has_value()) { return true; }
                resv = resv_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto period_tmp = StringUtils::fromString<uint32_t>(start, end);
                if(!period_tmp.has_value()) { return true; }
                period = period_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto pulseWidth_tmp = StringUtils::fromString<uint32_t>(start, end);
                if(!pulseWidth_tmp.has_value()) { return true; }
                pulseWidth = pulseWidth_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto offset_tmp = StringUtils::fromString<int32_t>(start, end);
                if(!offset_tmp.has_value()) { return true; }
                offset = offset_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage GnssSyncConfig::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%u,%u,%u,%u,%u,%u,%d", static_cast<uint8_t>(gnssSyncEnable),static_cast<uint8_t>(polarity),static_cast<uint8_t>(specType),resv,period,pulseWidth,offset);
                return result;
            }
            
            bool operator==(const GnssSyncConfig& lhs, const GnssSyncConfig& rhs)
            {
                return 
                (
                (lhs.gnssSyncEnable == rhs.gnssSyncEnable)
                && (lhs.polarity == rhs.polarity)
                && (lhs.specType == rhs.specType)
                && (lhs.resv == rhs.resv)
                && (lhs.period == rhs.period)
                && (lhs.pulseWidth == rhs.pulseWidth)
                && (lhs.offset == rhs.offset)
                );
            }
            
            bool Gnss2SolLla::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 15 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gps2Tow_tmp = StringUtils::fromString<double>(start, end);
                if(!gps2Tow_tmp.has_value()) { return true; }
                gps2Tow = gps2Tow_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gps2Week_tmp = StringUtils::fromString<uint16_t>(start, end);
                if(!gps2Week_tmp.has_value()) { return true; }
                gps2Week = gps2Week_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gnss2Fix_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!gnss2Fix_tmp.has_value()) { return true; }
                gnss2Fix = static_cast<Gnss2SolLla::Gnss2Fix>(gnss2Fix_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gnss2NumSats_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!gnss2NumSats_tmp.has_value()) { return true; }
                gnss2NumSats = gnss2NumSats_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto lat_tmp = StringUtils::fromString<double>(start, end);
                if(!lat_tmp.has_value()) { return true; }
                lat = lat_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto lon_tmp = StringUtils::fromString<double>(start, end);
                if(!lon_tmp.has_value()) { return true; }
                lon = lon_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto alt_tmp = StringUtils::fromString<double>(start, end);
                if(!alt_tmp.has_value()) { return true; }
                alt = alt_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velN_tmp = StringUtils::fromString<float>(start, end);
                if(!velN_tmp.has_value()) { return true; }
                velN = velN_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velE_tmp = StringUtils::fromString<float>(start, end);
                if(!velE_tmp.has_value()) { return true; }
                velE = velE_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velD_tmp = StringUtils::fromString<float>(start, end);
                if(!velD_tmp.has_value()) { return true; }
                velD = velD_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posUncertaintyN_tmp = StringUtils::fromString<float>(start, end);
                if(!posUncertaintyN_tmp.has_value()) { return true; }
                posUncertaintyN = posUncertaintyN_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posUncertaintyE_tmp = StringUtils::fromString<float>(start, end);
                if(!posUncertaintyE_tmp.has_value()) { return true; }
                posUncertaintyE = posUncertaintyE_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posUncertaintyD_tmp = StringUtils::fromString<float>(start, end);
                if(!posUncertaintyD_tmp.has_value()) { return true; }
                posUncertaintyD = posUncertaintyD_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gnss2VelUncertainty_tmp = StringUtils::fromString<float>(start, end);
                if(!gnss2VelUncertainty_tmp.has_value()) { return true; }
                gnss2VelUncertainty = gnss2VelUncertainty_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gnss2TimeUncertainty_tmp = StringUtils::fromString<float>(start, end);
                if(!gnss2TimeUncertainty_tmp.has_value()) { return true; }
                gnss2TimeUncertainty = gnss2TimeUncertainty_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const Gnss2SolLla& lhs, const Gnss2SolLla& rhs)
            {
                return 
                (
                (lhs.gps2Tow == rhs.gps2Tow)
                && (lhs.gps2Week == rhs.gps2Week)
                && (lhs.gnss2Fix == rhs.gnss2Fix)
                && (lhs.gnss2NumSats == rhs.gnss2NumSats)
                && (lhs.lat == rhs.lat)
                && (lhs.lon == rhs.lon)
                && (lhs.alt == rhs.alt)
                && (lhs.velN == rhs.velN)
                && (lhs.velE == rhs.velE)
                && (lhs.velD == rhs.velD)
                && (lhs.posUncertaintyN == rhs.posUncertaintyN)
                && (lhs.posUncertaintyE == rhs.posUncertaintyE)
                && (lhs.posUncertaintyD == rhs.posUncertaintyD)
                && (lhs.gnss2VelUncertainty == rhs.gnss2VelUncertainty)
                && (lhs.gnss2TimeUncertainty == rhs.gnss2TimeUncertainty)
                );
            }
            
            bool Gnss2SolEcef::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 15 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gps2Tow_tmp = StringUtils::fromString<double>(start, end);
                if(!gps2Tow_tmp.has_value()) { return true; }
                gps2Tow = gps2Tow_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gps2Week_tmp = StringUtils::fromString<uint16_t>(start, end);
                if(!gps2Week_tmp.has_value()) { return true; }
                gps2Week = gps2Week_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gnss2Fix_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!gnss2Fix_tmp.has_value()) { return true; }
                gnss2Fix = static_cast<Gnss2SolEcef::Gnss2Fix>(gnss2Fix_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gnss2NumSats_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!gnss2NumSats_tmp.has_value()) { return true; }
                gnss2NumSats = gnss2NumSats_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posX_tmp = StringUtils::fromString<double>(start, end);
                if(!posX_tmp.has_value()) { return true; }
                posX = posX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posY_tmp = StringUtils::fromString<double>(start, end);
                if(!posY_tmp.has_value()) { return true; }
                posY = posY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posZ_tmp = StringUtils::fromString<double>(start, end);
                if(!posZ_tmp.has_value()) { return true; }
                posZ = posZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velX_tmp = StringUtils::fromString<float>(start, end);
                if(!velX_tmp.has_value()) { return true; }
                velX = velX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velY_tmp = StringUtils::fromString<float>(start, end);
                if(!velY_tmp.has_value()) { return true; }
                velY = velY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velZ_tmp = StringUtils::fromString<float>(start, end);
                if(!velZ_tmp.has_value()) { return true; }
                velZ = velZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posUncertaintyX_tmp = StringUtils::fromString<float>(start, end);
                if(!posUncertaintyX_tmp.has_value()) { return true; }
                posUncertaintyX = posUncertaintyX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posUncertaintyY_tmp = StringUtils::fromString<float>(start, end);
                if(!posUncertaintyY_tmp.has_value()) { return true; }
                posUncertaintyY = posUncertaintyY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posUncertaintyZ_tmp = StringUtils::fromString<float>(start, end);
                if(!posUncertaintyZ_tmp.has_value()) { return true; }
                posUncertaintyZ = posUncertaintyZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gnss2VelUncertainty_tmp = StringUtils::fromString<float>(start, end);
                if(!gnss2VelUncertainty_tmp.has_value()) { return true; }
                gnss2VelUncertainty = gnss2VelUncertainty_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gnss2TimeUncertainty_tmp = StringUtils::fromString<float>(start, end);
                if(!gnss2TimeUncertainty_tmp.has_value()) { return true; }
                gnss2TimeUncertainty = gnss2TimeUncertainty_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const Gnss2SolEcef& lhs, const Gnss2SolEcef& rhs)
            {
                return 
                (
                (lhs.gps2Tow == rhs.gps2Tow)
                && (lhs.gps2Week == rhs.gps2Week)
                && (lhs.gnss2Fix == rhs.gnss2Fix)
                && (lhs.gnss2NumSats == rhs.gnss2NumSats)
                && (lhs.posX == rhs.posX)
                && (lhs.posY == rhs.posY)
                && (lhs.posZ == rhs.posZ)
                && (lhs.velX == rhs.velX)
                && (lhs.velY == rhs.velY)
                && (lhs.velZ == rhs.velZ)
                && (lhs.posUncertaintyX == rhs.posUncertaintyX)
                && (lhs.posUncertaintyY == rhs.posUncertaintyY)
                && (lhs.posUncertaintyZ == rhs.posUncertaintyZ)
                && (lhs.gnss2VelUncertainty == rhs.gnss2VelUncertainty)
                && (lhs.gnss2TimeUncertainty == rhs.gnss2TimeUncertainty)
                );
            }
            
            bool ExtGnssOffset::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 3 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto positionX_tmp = StringUtils::fromString<float>(start, end);
                if(!positionX_tmp.has_value()) { return true; }
                positionX = positionX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto positionY_tmp = StringUtils::fromString<float>(start, end);
                if(!positionY_tmp.has_value()) { return true; }
                positionY = positionY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto positionZ_tmp = StringUtils::fromString<float>(start, end);
                if(!positionZ_tmp.has_value()) { return true; }
                positionZ = positionZ_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage ExtGnssOffset::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%f,%f,%f", positionX,positionY,positionZ);
                return result;
            }
            
            bool operator==(const ExtGnssOffset& lhs, const ExtGnssOffset& rhs)
            {
                return 
                (
                (lhs.positionX == rhs.positionX)
                && (lhs.positionY == rhs.positionY)
                && (lhs.positionZ == rhs.positionZ)
                );
            }
            
        } // namespace GNSS
        
        namespace GNSSCompass
        {
            bool GnssCompassSignalHealthStatus::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 8 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto numSatsPvtA_tmp = StringUtils::fromString<float>(start, end);
                if(!numSatsPvtA_tmp.has_value()) { return true; }
                numSatsPvtA = numSatsPvtA_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto numSatsRtkA_tmp = StringUtils::fromString<float>(start, end);
                if(!numSatsRtkA_tmp.has_value()) { return true; }
                numSatsRtkA = numSatsRtkA_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto highestCn0A_tmp = StringUtils::fromString<float>(start, end);
                if(!highestCn0A_tmp.has_value()) { return true; }
                highestCn0A = highestCn0A_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto numSatsPvtB_tmp = StringUtils::fromString<float>(start, end);
                if(!numSatsPvtB_tmp.has_value()) { return true; }
                numSatsPvtB = numSatsPvtB_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto numSatsRtkB_tmp = StringUtils::fromString<float>(start, end);
                if(!numSatsRtkB_tmp.has_value()) { return true; }
                numSatsRtkB = numSatsRtkB_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto highestCn0B_tmp = StringUtils::fromString<float>(start, end);
                if(!highestCn0B_tmp.has_value()) { return true; }
                highestCn0B = highestCn0B_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto numComSatsPVT_tmp = StringUtils::fromString<float>(start, end);
                if(!numComSatsPVT_tmp.has_value()) { return true; }
                numComSatsPVT = numComSatsPVT_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto numComSatsRTK_tmp = StringUtils::fromString<float>(start, end);
                if(!numComSatsRTK_tmp.has_value()) { return true; }
                numComSatsRTK = numComSatsRTK_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const GnssCompassSignalHealthStatus& lhs, const GnssCompassSignalHealthStatus& rhs)
            {
                return 
                (
                (lhs.numSatsPvtA == rhs.numSatsPvtA)
                && (lhs.numSatsRtkA == rhs.numSatsRtkA)
                && (lhs.highestCn0A == rhs.highestCn0A)
                && (lhs.numSatsPvtB == rhs.numSatsPvtB)
                && (lhs.numSatsRtkB == rhs.numSatsRtkB)
                && (lhs.highestCn0B == rhs.highestCn0B)
                && (lhs.numComSatsPVT == rhs.numComSatsPVT)
                && (lhs.numComSatsRTK == rhs.numComSatsRTK)
                );
            }
            
            bool GnssCompassBaseline::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 6 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto positionX_tmp = StringUtils::fromString<float>(start, end);
                if(!positionX_tmp.has_value()) { return true; }
                positionX = positionX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto positionY_tmp = StringUtils::fromString<float>(start, end);
                if(!positionY_tmp.has_value()) { return true; }
                positionY = positionY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto positionZ_tmp = StringUtils::fromString<float>(start, end);
                if(!positionZ_tmp.has_value()) { return true; }
                positionZ = positionZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto uncertaintyX_tmp = StringUtils::fromString<float>(start, end);
                if(!uncertaintyX_tmp.has_value()) { return true; }
                uncertaintyX = uncertaintyX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto uncertaintyY_tmp = StringUtils::fromString<float>(start, end);
                if(!uncertaintyY_tmp.has_value()) { return true; }
                uncertaintyY = uncertaintyY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto uncertaintyZ_tmp = StringUtils::fromString<float>(start, end);
                if(!uncertaintyZ_tmp.has_value()) { return true; }
                uncertaintyZ = uncertaintyZ_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage GnssCompassBaseline::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%f,%f,%f,%f,%f,%f", positionX,positionY,positionZ,uncertaintyX,uncertaintyY,uncertaintyZ);
                return result;
            }
            
            bool operator==(const GnssCompassBaseline& lhs, const GnssCompassBaseline& rhs)
            {
                return 
                (
                (lhs.positionX == rhs.positionX)
                && (lhs.positionY == rhs.positionY)
                && (lhs.positionZ == rhs.positionZ)
                && (lhs.uncertaintyX == rhs.uncertaintyX)
                && (lhs.uncertaintyY == rhs.uncertaintyY)
                && (lhs.uncertaintyZ == rhs.uncertaintyZ)
                );
            }
            
            bool GnssCompassEstBaseline::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 9 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto estBaselineComplete_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!estBaselineComplete_tmp.has_value()) { return true; }
                estBaselineComplete = estBaselineComplete_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto resv_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!resv_tmp.has_value()) { return true; }
                resv = resv_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto numMeas_tmp = StringUtils::fromString<uint16_t>(start, end);
                if(!numMeas_tmp.has_value()) { return true; }
                numMeas = numMeas_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto positionX_tmp = StringUtils::fromString<float>(start, end);
                if(!positionX_tmp.has_value()) { return true; }
                positionX = positionX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto positionY_tmp = StringUtils::fromString<float>(start, end);
                if(!positionY_tmp.has_value()) { return true; }
                positionY = positionY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto positionZ_tmp = StringUtils::fromString<float>(start, end);
                if(!positionZ_tmp.has_value()) { return true; }
                positionZ = positionZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto uncertaintyX_tmp = StringUtils::fromString<float>(start, end);
                if(!uncertaintyX_tmp.has_value()) { return true; }
                uncertaintyX = uncertaintyX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto uncertaintyY_tmp = StringUtils::fromString<float>(start, end);
                if(!uncertaintyY_tmp.has_value()) { return true; }
                uncertaintyY = uncertaintyY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto uncertaintyZ_tmp = StringUtils::fromString<float>(start, end);
                if(!uncertaintyZ_tmp.has_value()) { return true; }
                uncertaintyZ = uncertaintyZ_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const GnssCompassEstBaseline& lhs, const GnssCompassEstBaseline& rhs)
            {
                return 
                (
                (lhs.estBaselineComplete == rhs.estBaselineComplete)
                && (lhs.resv == rhs.resv)
                && (lhs.numMeas == rhs.numMeas)
                && (lhs.positionX == rhs.positionX)
                && (lhs.positionY == rhs.positionY)
                && (lhs.positionZ == rhs.positionZ)
                && (lhs.uncertaintyX == rhs.uncertaintyX)
                && (lhs.uncertaintyY == rhs.uncertaintyY)
                && (lhs.uncertaintyZ == rhs.uncertaintyZ)
                );
            }
            
            bool GnssCompassStartupStatus::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 2 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto percentComplete_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!percentComplete_tmp.has_value()) { return true; }
                percentComplete = percentComplete_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto currentHeading_tmp = StringUtils::fromString<float>(start, end);
                if(!currentHeading_tmp.has_value()) { return true; }
                currentHeading = currentHeading_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const GnssCompassStartupStatus& lhs, const GnssCompassStartupStatus& rhs)
            {
                return 
                (
                (lhs.percentComplete == rhs.percentComplete)
                && (lhs.currentHeading == rhs.currentHeading)
                );
            }
            
        } // namespace GNSSCompass
        
        namespace HardSoftIronEstimator
        {
            bool RealTimeHsiControl::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 3 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto mode_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!mode_tmp.has_value()) { return true; }
                mode = static_cast<RealTimeHsiControl::Mode>(mode_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto applyCompensation_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!applyCompensation_tmp.has_value()) { return true; }
                applyCompensation = static_cast<RealTimeHsiControl::ApplyCompensation>(applyCompensation_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto convergeRate_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!convergeRate_tmp.has_value()) { return true; }
                convergeRate = convergeRate_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage RealTimeHsiControl::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%u,%u,%u", static_cast<uint8_t>(mode),static_cast<uint8_t>(applyCompensation),convergeRate);
                return result;
            }
            
            bool operator==(const RealTimeHsiControl& lhs, const RealTimeHsiControl& rhs)
            {
                return 
                (
                (lhs.mode == rhs.mode)
                && (lhs.applyCompensation == rhs.applyCompensation)
                && (lhs.convergeRate == rhs.convergeRate)
                );
            }
            
            bool EstMagCal::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 12 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magGain00_tmp = StringUtils::fromString<float>(start, end);
                if(!magGain00_tmp.has_value()) { return true; }
                magGain00 = magGain00_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magGain01_tmp = StringUtils::fromString<float>(start, end);
                if(!magGain01_tmp.has_value()) { return true; }
                magGain01 = magGain01_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magGain02_tmp = StringUtils::fromString<float>(start, end);
                if(!magGain02_tmp.has_value()) { return true; }
                magGain02 = magGain02_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magGain10_tmp = StringUtils::fromString<float>(start, end);
                if(!magGain10_tmp.has_value()) { return true; }
                magGain10 = magGain10_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magGain11_tmp = StringUtils::fromString<float>(start, end);
                if(!magGain11_tmp.has_value()) { return true; }
                magGain11 = magGain11_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magGain12_tmp = StringUtils::fromString<float>(start, end);
                if(!magGain12_tmp.has_value()) { return true; }
                magGain12 = magGain12_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magGain20_tmp = StringUtils::fromString<float>(start, end);
                if(!magGain20_tmp.has_value()) { return true; }
                magGain20 = magGain20_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magGain21_tmp = StringUtils::fromString<float>(start, end);
                if(!magGain21_tmp.has_value()) { return true; }
                magGain21 = magGain21_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magGain22_tmp = StringUtils::fromString<float>(start, end);
                if(!magGain22_tmp.has_value()) { return true; }
                magGain22 = magGain22_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magBiasX_tmp = StringUtils::fromString<float>(start, end);
                if(!magBiasX_tmp.has_value()) { return true; }
                magBiasX = magBiasX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magBiasY_tmp = StringUtils::fromString<float>(start, end);
                if(!magBiasY_tmp.has_value()) { return true; }
                magBiasY = magBiasY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magBiasZ_tmp = StringUtils::fromString<float>(start, end);
                if(!magBiasZ_tmp.has_value()) { return true; }
                magBiasZ = magBiasZ_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const EstMagCal& lhs, const EstMagCal& rhs)
            {
                return 
                (
                (lhs.magGain00 == rhs.magGain00)
                && (lhs.magGain01 == rhs.magGain01)
                && (lhs.magGain02 == rhs.magGain02)
                && (lhs.magGain10 == rhs.magGain10)
                && (lhs.magGain11 == rhs.magGain11)
                && (lhs.magGain12 == rhs.magGain12)
                && (lhs.magGain20 == rhs.magGain20)
                && (lhs.magGain21 == rhs.magGain21)
                && (lhs.magGain22 == rhs.magGain22)
                && (lhs.magBiasX == rhs.magBiasX)
                && (lhs.magBiasY == rhs.magBiasY)
                && (lhs.magBiasZ == rhs.magBiasZ)
                );
            }
            
        } // namespace HardSoftIronEstimator
        
        namespace Heave
        {
            bool HeaveOutputs::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 3 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto heave_tmp = StringUtils::fromString<float>(start, end);
                if(!heave_tmp.has_value()) { return true; }
                heave = heave_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto heaveRate_tmp = StringUtils::fromString<float>(start, end);
                if(!heaveRate_tmp.has_value()) { return true; }
                heaveRate = heaveRate_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto delayedHeave_tmp = StringUtils::fromString<float>(start, end);
                if(!delayedHeave_tmp.has_value()) { return true; }
                delayedHeave = delayedHeave_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const HeaveOutputs& lhs, const HeaveOutputs& rhs)
            {
                return 
                (
                (lhs.heave == rhs.heave)
                && (lhs.heaveRate == rhs.heaveRate)
                && (lhs.delayedHeave == rhs.delayedHeave)
                );
            }
            
            bool HeaveBasicConfig::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 7 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto initialWavePeriod_tmp = StringUtils::fromString<float>(start, end);
                if(!initialWavePeriod_tmp.has_value()) { return true; }
                initialWavePeriod = initialWavePeriod_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto initialWaveAmplitude_tmp = StringUtils::fromString<float>(start, end);
                if(!initialWaveAmplitude_tmp.has_value()) { return true; }
                initialWaveAmplitude = initialWaveAmplitude_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto maxWavePeriod_tmp = StringUtils::fromString<float>(start, end);
                if(!maxWavePeriod_tmp.has_value()) { return true; }
                maxWavePeriod = maxWavePeriod_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto minWaveAmplitude_tmp = StringUtils::fromString<float>(start, end);
                if(!minWaveAmplitude_tmp.has_value()) { return true; }
                minWaveAmplitude = minWaveAmplitude_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto delayedHeaveCutoffFreq_tmp = StringUtils::fromString<float>(start, end);
                if(!delayedHeaveCutoffFreq_tmp.has_value()) { return true; }
                delayedHeaveCutoffFreq = delayedHeaveCutoffFreq_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto heaveCutoffFreq_tmp = StringUtils::fromString<float>(start, end);
                if(!heaveCutoffFreq_tmp.has_value()) { return true; }
                heaveCutoffFreq = heaveCutoffFreq_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto heaveRateCutoffFreq_tmp = StringUtils::fromString<float>(start, end);
                if(!heaveRateCutoffFreq_tmp.has_value()) { return true; }
                heaveRateCutoffFreq = heaveRateCutoffFreq_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage HeaveBasicConfig::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%f,%f,%f,%f,%f,%f,%f", initialWavePeriod,initialWaveAmplitude,maxWavePeriod,minWaveAmplitude,delayedHeaveCutoffFreq,heaveCutoffFreq,heaveRateCutoffFreq);
                return result;
            }
            
            bool operator==(const HeaveBasicConfig& lhs, const HeaveBasicConfig& rhs)
            {
                return 
                (
                (lhs.initialWavePeriod == rhs.initialWavePeriod)
                && (lhs.initialWaveAmplitude == rhs.initialWaveAmplitude)
                && (lhs.maxWavePeriod == rhs.maxWavePeriod)
                && (lhs.minWaveAmplitude == rhs.minWaveAmplitude)
                && (lhs.delayedHeaveCutoffFreq == rhs.delayedHeaveCutoffFreq)
                && (lhs.heaveCutoffFreq == rhs.heaveCutoffFreq)
                && (lhs.heaveRateCutoffFreq == rhs.heaveRateCutoffFreq)
                );
            }
            
        } // namespace Heave
        
        namespace IMU
        {
            bool Mag::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 3 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magX_tmp = StringUtils::fromString<float>(start, end);
                if(!magX_tmp.has_value()) { return true; }
                magX = magX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magY_tmp = StringUtils::fromString<float>(start, end);
                if(!magY_tmp.has_value()) { return true; }
                magY = magY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magZ_tmp = StringUtils::fromString<float>(start, end);
                if(!magZ_tmp.has_value()) { return true; }
                magZ = magZ_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const Mag& lhs, const Mag& rhs)
            {
                return 
                (
                (lhs.magX == rhs.magX)
                && (lhs.magY == rhs.magY)
                && (lhs.magZ == rhs.magZ)
                );
            }
            
            bool Accel::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 3 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelX_tmp = StringUtils::fromString<float>(start, end);
                if(!accelX_tmp.has_value()) { return true; }
                accelX = accelX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelY_tmp = StringUtils::fromString<float>(start, end);
                if(!accelY_tmp.has_value()) { return true; }
                accelY = accelY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelZ_tmp = StringUtils::fromString<float>(start, end);
                if(!accelZ_tmp.has_value()) { return true; }
                accelZ = accelZ_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const Accel& lhs, const Accel& rhs)
            {
                return 
                (
                (lhs.accelX == rhs.accelX)
                && (lhs.accelY == rhs.accelY)
                && (lhs.accelZ == rhs.accelZ)
                );
            }
            
            bool Gyro::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 3 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroX_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroX_tmp.has_value()) { return true; }
                gyroX = gyroX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroY_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroY_tmp.has_value()) { return true; }
                gyroY = gyroY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroZ_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroZ_tmp.has_value()) { return true; }
                gyroZ = gyroZ_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const Gyro& lhs, const Gyro& rhs)
            {
                return 
                (
                (lhs.gyroX == rhs.gyroX)
                && (lhs.gyroY == rhs.gyroY)
                && (lhs.gyroZ == rhs.gyroZ)
                );
            }
            
            bool MagAccelGyro::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 9 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magX_tmp = StringUtils::fromString<float>(start, end);
                if(!magX_tmp.has_value()) { return true; }
                magX = magX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magY_tmp = StringUtils::fromString<float>(start, end);
                if(!magY_tmp.has_value()) { return true; }
                magY = magY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magZ_tmp = StringUtils::fromString<float>(start, end);
                if(!magZ_tmp.has_value()) { return true; }
                magZ = magZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelX_tmp = StringUtils::fromString<float>(start, end);
                if(!accelX_tmp.has_value()) { return true; }
                accelX = accelX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelY_tmp = StringUtils::fromString<float>(start, end);
                if(!accelY_tmp.has_value()) { return true; }
                accelY = accelY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelZ_tmp = StringUtils::fromString<float>(start, end);
                if(!accelZ_tmp.has_value()) { return true; }
                accelZ = accelZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroX_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroX_tmp.has_value()) { return true; }
                gyroX = gyroX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroY_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroY_tmp.has_value()) { return true; }
                gyroY = gyroY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroZ_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroZ_tmp.has_value()) { return true; }
                gyroZ = gyroZ_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const MagAccelGyro& lhs, const MagAccelGyro& rhs)
            {
                return 
                (
                (lhs.magX == rhs.magX)
                && (lhs.magY == rhs.magY)
                && (lhs.magZ == rhs.magZ)
                && (lhs.accelX == rhs.accelX)
                && (lhs.accelY == rhs.accelY)
                && (lhs.accelZ == rhs.accelZ)
                && (lhs.gyroX == rhs.gyroX)
                && (lhs.gyroY == rhs.gyroY)
                && (lhs.gyroZ == rhs.gyroZ)
                );
            }
            
            bool MagCal::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 12 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magGain00_tmp = StringUtils::fromString<float>(start, end);
                if(!magGain00_tmp.has_value()) { return true; }
                magGain00 = magGain00_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magGain01_tmp = StringUtils::fromString<float>(start, end);
                if(!magGain01_tmp.has_value()) { return true; }
                magGain01 = magGain01_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magGain02_tmp = StringUtils::fromString<float>(start, end);
                if(!magGain02_tmp.has_value()) { return true; }
                magGain02 = magGain02_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magGain10_tmp = StringUtils::fromString<float>(start, end);
                if(!magGain10_tmp.has_value()) { return true; }
                magGain10 = magGain10_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magGain11_tmp = StringUtils::fromString<float>(start, end);
                if(!magGain11_tmp.has_value()) { return true; }
                magGain11 = magGain11_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magGain12_tmp = StringUtils::fromString<float>(start, end);
                if(!magGain12_tmp.has_value()) { return true; }
                magGain12 = magGain12_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magGain20_tmp = StringUtils::fromString<float>(start, end);
                if(!magGain20_tmp.has_value()) { return true; }
                magGain20 = magGain20_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magGain21_tmp = StringUtils::fromString<float>(start, end);
                if(!magGain21_tmp.has_value()) { return true; }
                magGain21 = magGain21_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magGain22_tmp = StringUtils::fromString<float>(start, end);
                if(!magGain22_tmp.has_value()) { return true; }
                magGain22 = magGain22_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magBiasX_tmp = StringUtils::fromString<float>(start, end);
                if(!magBiasX_tmp.has_value()) { return true; }
                magBiasX = magBiasX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magBiasY_tmp = StringUtils::fromString<float>(start, end);
                if(!magBiasY_tmp.has_value()) { return true; }
                magBiasY = magBiasY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magBiasZ_tmp = StringUtils::fromString<float>(start, end);
                if(!magBiasZ_tmp.has_value()) { return true; }
                magBiasZ = magBiasZ_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage MagCal::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f", magGain00,magGain01,magGain02,magGain10,magGain11,magGain12,magGain20,magGain21,magGain22,magBiasX,magBiasY,magBiasZ);
                return result;
            }
            
            bool operator==(const MagCal& lhs, const MagCal& rhs)
            {
                return 
                (
                (lhs.magGain00 == rhs.magGain00)
                && (lhs.magGain01 == rhs.magGain01)
                && (lhs.magGain02 == rhs.magGain02)
                && (lhs.magGain10 == rhs.magGain10)
                && (lhs.magGain11 == rhs.magGain11)
                && (lhs.magGain12 == rhs.magGain12)
                && (lhs.magGain20 == rhs.magGain20)
                && (lhs.magGain21 == rhs.magGain21)
                && (lhs.magGain22 == rhs.magGain22)
                && (lhs.magBiasX == rhs.magBiasX)
                && (lhs.magBiasY == rhs.magBiasY)
                && (lhs.magBiasZ == rhs.magBiasZ)
                );
            }
            
            bool AccelCal::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 12 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelGain00_tmp = StringUtils::fromString<float>(start, end);
                if(!accelGain00_tmp.has_value()) { return true; }
                accelGain00 = accelGain00_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelGain01_tmp = StringUtils::fromString<float>(start, end);
                if(!accelGain01_tmp.has_value()) { return true; }
                accelGain01 = accelGain01_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelGain02_tmp = StringUtils::fromString<float>(start, end);
                if(!accelGain02_tmp.has_value()) { return true; }
                accelGain02 = accelGain02_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelGain10_tmp = StringUtils::fromString<float>(start, end);
                if(!accelGain10_tmp.has_value()) { return true; }
                accelGain10 = accelGain10_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelGain11_tmp = StringUtils::fromString<float>(start, end);
                if(!accelGain11_tmp.has_value()) { return true; }
                accelGain11 = accelGain11_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelGain12_tmp = StringUtils::fromString<float>(start, end);
                if(!accelGain12_tmp.has_value()) { return true; }
                accelGain12 = accelGain12_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelGain20_tmp = StringUtils::fromString<float>(start, end);
                if(!accelGain20_tmp.has_value()) { return true; }
                accelGain20 = accelGain20_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelGain21_tmp = StringUtils::fromString<float>(start, end);
                if(!accelGain21_tmp.has_value()) { return true; }
                accelGain21 = accelGain21_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelGain22_tmp = StringUtils::fromString<float>(start, end);
                if(!accelGain22_tmp.has_value()) { return true; }
                accelGain22 = accelGain22_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelBiasX_tmp = StringUtils::fromString<float>(start, end);
                if(!accelBiasX_tmp.has_value()) { return true; }
                accelBiasX = accelBiasX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelBiasY_tmp = StringUtils::fromString<float>(start, end);
                if(!accelBiasY_tmp.has_value()) { return true; }
                accelBiasY = accelBiasY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelBiasZ_tmp = StringUtils::fromString<float>(start, end);
                if(!accelBiasZ_tmp.has_value()) { return true; }
                accelBiasZ = accelBiasZ_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage AccelCal::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f", accelGain00,accelGain01,accelGain02,accelGain10,accelGain11,accelGain12,accelGain20,accelGain21,accelGain22,accelBiasX,accelBiasY,accelBiasZ);
                return result;
            }
            
            bool operator==(const AccelCal& lhs, const AccelCal& rhs)
            {
                return 
                (
                (lhs.accelGain00 == rhs.accelGain00)
                && (lhs.accelGain01 == rhs.accelGain01)
                && (lhs.accelGain02 == rhs.accelGain02)
                && (lhs.accelGain10 == rhs.accelGain10)
                && (lhs.accelGain11 == rhs.accelGain11)
                && (lhs.accelGain12 == rhs.accelGain12)
                && (lhs.accelGain20 == rhs.accelGain20)
                && (lhs.accelGain21 == rhs.accelGain21)
                && (lhs.accelGain22 == rhs.accelGain22)
                && (lhs.accelBiasX == rhs.accelBiasX)
                && (lhs.accelBiasY == rhs.accelBiasY)
                && (lhs.accelBiasZ == rhs.accelBiasZ)
                );
            }
            
            bool RefFrameRot::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 9 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto rFR00_tmp = StringUtils::fromString<float>(start, end);
                if(!rFR00_tmp.has_value()) { return true; }
                rFR00 = rFR00_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto rFR01_tmp = StringUtils::fromString<float>(start, end);
                if(!rFR01_tmp.has_value()) { return true; }
                rFR01 = rFR01_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto rFR02_tmp = StringUtils::fromString<float>(start, end);
                if(!rFR02_tmp.has_value()) { return true; }
                rFR02 = rFR02_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto rFR10_tmp = StringUtils::fromString<float>(start, end);
                if(!rFR10_tmp.has_value()) { return true; }
                rFR10 = rFR10_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto rFR11_tmp = StringUtils::fromString<float>(start, end);
                if(!rFR11_tmp.has_value()) { return true; }
                rFR11 = rFR11_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto rFR12_tmp = StringUtils::fromString<float>(start, end);
                if(!rFR12_tmp.has_value()) { return true; }
                rFR12 = rFR12_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto rFR20_tmp = StringUtils::fromString<float>(start, end);
                if(!rFR20_tmp.has_value()) { return true; }
                rFR20 = rFR20_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto rFR21_tmp = StringUtils::fromString<float>(start, end);
                if(!rFR21_tmp.has_value()) { return true; }
                rFR21 = rFR21_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto rFR22_tmp = StringUtils::fromString<float>(start, end);
                if(!rFR22_tmp.has_value()) { return true; }
                rFR22 = rFR22_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage RefFrameRot::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%f,%f,%f,%f,%f,%f,%f,%f,%f", rFR00,rFR01,rFR02,rFR10,rFR11,rFR12,rFR20,rFR21,rFR22);
                return result;
            }
            
            bool operator==(const RefFrameRot& lhs, const RefFrameRot& rhs)
            {
                return 
                (
                (lhs.rFR00 == rhs.rFR00)
                && (lhs.rFR01 == rhs.rFR01)
                && (lhs.rFR02 == rhs.rFR02)
                && (lhs.rFR10 == rhs.rFR10)
                && (lhs.rFR11 == rhs.rFR11)
                && (lhs.rFR12 == rhs.rFR12)
                && (lhs.rFR20 == rhs.rFR20)
                && (lhs.rFR21 == rhs.rFR21)
                && (lhs.rFR22 == rhs.rFR22)
                );
            }
            
            bool ImuMeas::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 11 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto uncompMagX_tmp = StringUtils::fromString<float>(start, end);
                if(!uncompMagX_tmp.has_value()) { return true; }
                uncompMagX = uncompMagX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto uncompMagY_tmp = StringUtils::fromString<float>(start, end);
                if(!uncompMagY_tmp.has_value()) { return true; }
                uncompMagY = uncompMagY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto uncompMagZ_tmp = StringUtils::fromString<float>(start, end);
                if(!uncompMagZ_tmp.has_value()) { return true; }
                uncompMagZ = uncompMagZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto uncompAccX_tmp = StringUtils::fromString<float>(start, end);
                if(!uncompAccX_tmp.has_value()) { return true; }
                uncompAccX = uncompAccX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto uncompAccY_tmp = StringUtils::fromString<float>(start, end);
                if(!uncompAccY_tmp.has_value()) { return true; }
                uncompAccY = uncompAccY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto uncompAccZ_tmp = StringUtils::fromString<float>(start, end);
                if(!uncompAccZ_tmp.has_value()) { return true; }
                uncompAccZ = uncompAccZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto uncompGyroX_tmp = StringUtils::fromString<float>(start, end);
                if(!uncompGyroX_tmp.has_value()) { return true; }
                uncompGyroX = uncompGyroX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto uncompGyroY_tmp = StringUtils::fromString<float>(start, end);
                if(!uncompGyroY_tmp.has_value()) { return true; }
                uncompGyroY = uncompGyroY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto uncompGyroZ_tmp = StringUtils::fromString<float>(start, end);
                if(!uncompGyroZ_tmp.has_value()) { return true; }
                uncompGyroZ = uncompGyroZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto temperature_tmp = StringUtils::fromString<float>(start, end);
                if(!temperature_tmp.has_value()) { return true; }
                temperature = temperature_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto pressure_tmp = StringUtils::fromString<float>(start, end);
                if(!pressure_tmp.has_value()) { return true; }
                pressure = pressure_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const ImuMeas& lhs, const ImuMeas& rhs)
            {
                return 
                (
                (lhs.uncompMagX == rhs.uncompMagX)
                && (lhs.uncompMagY == rhs.uncompMagY)
                && (lhs.uncompMagZ == rhs.uncompMagZ)
                && (lhs.uncompAccX == rhs.uncompAccX)
                && (lhs.uncompAccY == rhs.uncompAccY)
                && (lhs.uncompAccZ == rhs.uncompAccZ)
                && (lhs.uncompGyroX == rhs.uncompGyroX)
                && (lhs.uncompGyroY == rhs.uncompGyroY)
                && (lhs.uncompGyroZ == rhs.uncompGyroZ)
                && (lhs.temperature == rhs.temperature)
                && (lhs.pressure == rhs.pressure)
                );
            }
            
            bool DeltaThetaVelocity::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 7 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto deltaTime_tmp = StringUtils::fromString<float>(start, end);
                if(!deltaTime_tmp.has_value()) { return true; }
                deltaTime = deltaTime_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto deltaThetaX_tmp = StringUtils::fromString<float>(start, end);
                if(!deltaThetaX_tmp.has_value()) { return true; }
                deltaThetaX = deltaThetaX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto deltaThetaY_tmp = StringUtils::fromString<float>(start, end);
                if(!deltaThetaY_tmp.has_value()) { return true; }
                deltaThetaY = deltaThetaY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto deltaThetaZ_tmp = StringUtils::fromString<float>(start, end);
                if(!deltaThetaZ_tmp.has_value()) { return true; }
                deltaThetaZ = deltaThetaZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto deltaVelX_tmp = StringUtils::fromString<float>(start, end);
                if(!deltaVelX_tmp.has_value()) { return true; }
                deltaVelX = deltaVelX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto deltaVelY_tmp = StringUtils::fromString<float>(start, end);
                if(!deltaVelY_tmp.has_value()) { return true; }
                deltaVelY = deltaVelY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto deltaVelZ_tmp = StringUtils::fromString<float>(start, end);
                if(!deltaVelZ_tmp.has_value()) { return true; }
                deltaVelZ = deltaVelZ_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const DeltaThetaVelocity& lhs, const DeltaThetaVelocity& rhs)
            {
                return 
                (
                (lhs.deltaTime == rhs.deltaTime)
                && (lhs.deltaThetaX == rhs.deltaThetaX)
                && (lhs.deltaThetaY == rhs.deltaThetaY)
                && (lhs.deltaThetaZ == rhs.deltaThetaZ)
                && (lhs.deltaVelX == rhs.deltaVelX)
                && (lhs.deltaVelY == rhs.deltaVelY)
                && (lhs.deltaVelZ == rhs.deltaVelZ)
                );
            }
            
            bool DeltaThetaVelConfig::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 5 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto integrationFrame_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!integrationFrame_tmp.has_value()) { return true; }
                integrationFrame = static_cast<DeltaThetaVelConfig::IntegrationFrame>(integrationFrame_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroCompensation_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!gyroCompensation_tmp.has_value()) { return true; }
                gyroCompensation = static_cast<DeltaThetaVelConfig::GyroCompensation>(gyroCompensation_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelCompensation_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!accelCompensation_tmp.has_value()) { return true; }
                accelCompensation = static_cast<DeltaThetaVelConfig::AccelCompensation>(accelCompensation_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto earthRateCompensation_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!earthRateCompensation_tmp.has_value()) { return true; }
                earthRateCompensation = static_cast<DeltaThetaVelConfig::EarthRateCompensation>(earthRateCompensation_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto resv_tmp = StringUtils::fromString<uint16_t>(start, end);
                if(!resv_tmp.has_value()) { return true; }
                resv = resv_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage DeltaThetaVelConfig::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%u,%u,%u,%u,%u", static_cast<uint8_t>(integrationFrame),static_cast<uint8_t>(gyroCompensation),static_cast<uint8_t>(accelCompensation),static_cast<uint8_t>(earthRateCompensation),resv);
                return result;
            }
            
            bool operator==(const DeltaThetaVelConfig& lhs, const DeltaThetaVelConfig& rhs)
            {
                return 
                (
                (lhs.integrationFrame == rhs.integrationFrame)
                && (lhs.gyroCompensation == rhs.gyroCompensation)
                && (lhs.accelCompensation == rhs.accelCompensation)
                && (lhs.earthRateCompensation == rhs.earthRateCompensation)
                && (lhs.resv == rhs.resv)
                );
            }
            
            bool GyroCal::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 12 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroGain00_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroGain00_tmp.has_value()) { return true; }
                gyroGain00 = gyroGain00_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroGain01_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroGain01_tmp.has_value()) { return true; }
                gyroGain01 = gyroGain01_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroGain02_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroGain02_tmp.has_value()) { return true; }
                gyroGain02 = gyroGain02_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroGain10_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroGain10_tmp.has_value()) { return true; }
                gyroGain10 = gyroGain10_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroGain11_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroGain11_tmp.has_value()) { return true; }
                gyroGain11 = gyroGain11_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroGain12_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroGain12_tmp.has_value()) { return true; }
                gyroGain12 = gyroGain12_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroGain20_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroGain20_tmp.has_value()) { return true; }
                gyroGain20 = gyroGain20_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroGain21_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroGain21_tmp.has_value()) { return true; }
                gyroGain21 = gyroGain21_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroGain22_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroGain22_tmp.has_value()) { return true; }
                gyroGain22 = gyroGain22_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroBiasX_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroBiasX_tmp.has_value()) { return true; }
                gyroBiasX = gyroBiasX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroBiasY_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroBiasY_tmp.has_value()) { return true; }
                gyroBiasY = gyroBiasY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroBiasZ_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroBiasZ_tmp.has_value()) { return true; }
                gyroBiasZ = gyroBiasZ_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage GyroCal::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f", gyroGain00,gyroGain01,gyroGain02,gyroGain10,gyroGain11,gyroGain12,gyroGain20,gyroGain21,gyroGain22,gyroBiasX,gyroBiasY,gyroBiasZ);
                return result;
            }
            
            bool operator==(const GyroCal& lhs, const GyroCal& rhs)
            {
                return 
                (
                (lhs.gyroGain00 == rhs.gyroGain00)
                && (lhs.gyroGain01 == rhs.gyroGain01)
                && (lhs.gyroGain02 == rhs.gyroGain02)
                && (lhs.gyroGain10 == rhs.gyroGain10)
                && (lhs.gyroGain11 == rhs.gyroGain11)
                && (lhs.gyroGain12 == rhs.gyroGain12)
                && (lhs.gyroGain20 == rhs.gyroGain20)
                && (lhs.gyroGain21 == rhs.gyroGain21)
                && (lhs.gyroGain22 == rhs.gyroGain22)
                && (lhs.gyroBiasX == rhs.gyroBiasX)
                && (lhs.gyroBiasY == rhs.gyroBiasY)
                && (lhs.gyroBiasZ == rhs.gyroBiasZ)
                );
            }
            
            bool ImuFilterControl::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 10 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magWindowSize_tmp = StringUtils::fromString<uint16_t>(start, end);
                if(!magWindowSize_tmp.has_value()) { return true; }
                magWindowSize = magWindowSize_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelWindowSize_tmp = StringUtils::fromString<uint16_t>(start, end);
                if(!accelWindowSize_tmp.has_value()) { return true; }
                accelWindowSize = accelWindowSize_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroWindowSize_tmp = StringUtils::fromString<uint16_t>(start, end);
                if(!gyroWindowSize_tmp.has_value()) { return true; }
                gyroWindowSize = gyroWindowSize_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto tempWindowSize_tmp = StringUtils::fromString<uint16_t>(start, end);
                if(!tempWindowSize_tmp.has_value()) { return true; }
                tempWindowSize = tempWindowSize_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto presWindowSize_tmp = StringUtils::fromString<uint16_t>(start, end);
                if(!presWindowSize_tmp.has_value()) { return true; }
                presWindowSize = presWindowSize_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto magFilterMode_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!magFilterMode_tmp.has_value()) { return true; }
                magFilterMode = magFilterMode_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelFilterMode_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!accelFilterMode_tmp.has_value()) { return true; }
                accelFilterMode = accelFilterMode_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroFilterMode_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!gyroFilterMode_tmp.has_value()) { return true; }
                gyroFilterMode = gyroFilterMode_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto tempFilterMode_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!tempFilterMode_tmp.has_value()) { return true; }
                tempFilterMode = tempFilterMode_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto presFilterMode_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!presFilterMode_tmp.has_value()) { return true; }
                presFilterMode = presFilterMode_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage ImuFilterControl::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%u,%u,%u,%u,%u,%u,%u,%u,%u,%u", magWindowSize,accelWindowSize,gyroWindowSize,tempWindowSize,presWindowSize,uint8_t(magFilterMode),uint8_t(accelFilterMode),uint8_t(gyroFilterMode),uint8_t(tempFilterMode),uint8_t(presFilterMode));
                return result;
            }
            
            bool operator==(const ImuFilterControl& lhs, const ImuFilterControl& rhs)
            {
                return 
                (
                (lhs.magWindowSize == rhs.magWindowSize)
                && (lhs.accelWindowSize == rhs.accelWindowSize)
                && (lhs.gyroWindowSize == rhs.gyroWindowSize)
                && (lhs.tempWindowSize == rhs.tempWindowSize)
                && (lhs.presWindowSize == rhs.presWindowSize)
                && (uint8_t(lhs.magFilterMode) == uint8_t(rhs.magFilterMode))
                && (uint8_t(lhs.accelFilterMode) == uint8_t(rhs.accelFilterMode))
                && (uint8_t(lhs.gyroFilterMode) == uint8_t(rhs.gyroFilterMode))
                && (uint8_t(lhs.tempFilterMode) == uint8_t(rhs.tempFilterMode))
                && (uint8_t(lhs.presFilterMode) == uint8_t(rhs.presFilterMode))
                );
            }
            
        } // namespace IMU
        
        namespace INS
        {
            bool InsSolLla::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 15 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto timeGpsTow_tmp = StringUtils::fromString<double>(start, end);
                if(!timeGpsTow_tmp.has_value()) { return true; }
                timeGpsTow = timeGpsTow_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto timeGpsWeek_tmp = StringUtils::fromString<uint16_t>(start, end);
                if(!timeGpsWeek_tmp.has_value()) { return true; }
                timeGpsWeek = timeGpsWeek_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto insStatus_tmp = StringUtils::fromStringHex<uint16_t>(start, end);
                if(!insStatus_tmp.has_value()) { return true; }
                insStatus = insStatus_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto yaw_tmp = StringUtils::fromString<float>(start, end);
                if(!yaw_tmp.has_value()) { return true; }
                yaw = yaw_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto pitch_tmp = StringUtils::fromString<float>(start, end);
                if(!pitch_tmp.has_value()) { return true; }
                pitch = pitch_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto roll_tmp = StringUtils::fromString<float>(start, end);
                if(!roll_tmp.has_value()) { return true; }
                roll = roll_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posLat_tmp = StringUtils::fromString<double>(start, end);
                if(!posLat_tmp.has_value()) { return true; }
                posLat = posLat_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posLon_tmp = StringUtils::fromString<double>(start, end);
                if(!posLon_tmp.has_value()) { return true; }
                posLon = posLon_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posAlt_tmp = StringUtils::fromString<double>(start, end);
                if(!posAlt_tmp.has_value()) { return true; }
                posAlt = posAlt_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velN_tmp = StringUtils::fromString<float>(start, end);
                if(!velN_tmp.has_value()) { return true; }
                velN = velN_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velE_tmp = StringUtils::fromString<float>(start, end);
                if(!velE_tmp.has_value()) { return true; }
                velE = velE_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velD_tmp = StringUtils::fromString<float>(start, end);
                if(!velD_tmp.has_value()) { return true; }
                velD = velD_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto attUncertainty_tmp = StringUtils::fromString<float>(start, end);
                if(!attUncertainty_tmp.has_value()) { return true; }
                attUncertainty = attUncertainty_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posUncertainty_tmp = StringUtils::fromString<float>(start, end);
                if(!posUncertainty_tmp.has_value()) { return true; }
                posUncertainty = posUncertainty_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velUncertainty_tmp = StringUtils::fromString<float>(start, end);
                if(!velUncertainty_tmp.has_value()) { return true; }
                velUncertainty = velUncertainty_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const InsSolLla& lhs, const InsSolLla& rhs)
            {
                return 
                (
                (lhs.timeGpsTow == rhs.timeGpsTow)
                && (lhs.timeGpsWeek == rhs.timeGpsWeek)
                && (uint16_t(lhs.insStatus) == uint16_t(rhs.insStatus))
                && (lhs.yaw == rhs.yaw)
                && (lhs.pitch == rhs.pitch)
                && (lhs.roll == rhs.roll)
                && (lhs.posLat == rhs.posLat)
                && (lhs.posLon == rhs.posLon)
                && (lhs.posAlt == rhs.posAlt)
                && (lhs.velN == rhs.velN)
                && (lhs.velE == rhs.velE)
                && (lhs.velD == rhs.velD)
                && (lhs.attUncertainty == rhs.attUncertainty)
                && (lhs.posUncertainty == rhs.posUncertainty)
                && (lhs.velUncertainty == rhs.velUncertainty)
                );
            }
            
            bool InsSolEcef::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 15 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto timeGpsTow_tmp = StringUtils::fromString<double>(start, end);
                if(!timeGpsTow_tmp.has_value()) { return true; }
                timeGpsTow = timeGpsTow_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto timeGpsWeek_tmp = StringUtils::fromString<uint16_t>(start, end);
                if(!timeGpsWeek_tmp.has_value()) { return true; }
                timeGpsWeek = timeGpsWeek_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto insStatus_tmp = StringUtils::fromStringHex<uint16_t>(start, end);
                if(!insStatus_tmp.has_value()) { return true; }
                insStatus = insStatus_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto yaw_tmp = StringUtils::fromString<float>(start, end);
                if(!yaw_tmp.has_value()) { return true; }
                yaw = yaw_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto pitch_tmp = StringUtils::fromString<float>(start, end);
                if(!pitch_tmp.has_value()) { return true; }
                pitch = pitch_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto roll_tmp = StringUtils::fromString<float>(start, end);
                if(!roll_tmp.has_value()) { return true; }
                roll = roll_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posEX_tmp = StringUtils::fromString<double>(start, end);
                if(!posEX_tmp.has_value()) { return true; }
                posEX = posEX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posEY_tmp = StringUtils::fromString<double>(start, end);
                if(!posEY_tmp.has_value()) { return true; }
                posEY = posEY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posEZ_tmp = StringUtils::fromString<double>(start, end);
                if(!posEZ_tmp.has_value()) { return true; }
                posEZ = posEZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velEX_tmp = StringUtils::fromString<float>(start, end);
                if(!velEX_tmp.has_value()) { return true; }
                velEX = velEX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velEY_tmp = StringUtils::fromString<float>(start, end);
                if(!velEY_tmp.has_value()) { return true; }
                velEY = velEY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velEZ_tmp = StringUtils::fromString<float>(start, end);
                if(!velEZ_tmp.has_value()) { return true; }
                velEZ = velEZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto attUncertainty_tmp = StringUtils::fromString<float>(start, end);
                if(!attUncertainty_tmp.has_value()) { return true; }
                attUncertainty = attUncertainty_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posUncertainty_tmp = StringUtils::fromString<float>(start, end);
                if(!posUncertainty_tmp.has_value()) { return true; }
                posUncertainty = posUncertainty_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velUncertainty_tmp = StringUtils::fromString<float>(start, end);
                if(!velUncertainty_tmp.has_value()) { return true; }
                velUncertainty = velUncertainty_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const InsSolEcef& lhs, const InsSolEcef& rhs)
            {
                return 
                (
                (lhs.timeGpsTow == rhs.timeGpsTow)
                && (lhs.timeGpsWeek == rhs.timeGpsWeek)
                && (uint16_t(lhs.insStatus) == uint16_t(rhs.insStatus))
                && (lhs.yaw == rhs.yaw)
                && (lhs.pitch == rhs.pitch)
                && (lhs.roll == rhs.roll)
                && (lhs.posEX == rhs.posEX)
                && (lhs.posEY == rhs.posEY)
                && (lhs.posEZ == rhs.posEZ)
                && (lhs.velEX == rhs.velEX)
                && (lhs.velEY == rhs.velEY)
                && (lhs.velEZ == rhs.velEZ)
                && (lhs.attUncertainty == rhs.attUncertainty)
                && (lhs.posUncertainty == rhs.posUncertainty)
                && (lhs.velUncertainty == rhs.velUncertainty)
                );
            }
            
            bool InsBasicConfig::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 4 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto scenario_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!scenario_tmp.has_value()) { return true; }
                scenario = static_cast<InsBasicConfig::Scenario>(scenario_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto ahrsAiding_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!ahrsAiding_tmp.has_value()) { return true; }
                ahrsAiding = static_cast<InsBasicConfig::AhrsAiding>(ahrsAiding_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto estBaseline_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!estBaseline_tmp.has_value()) { return true; }
                estBaseline = static_cast<InsBasicConfig::EstBaseline>(estBaseline_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto resv_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!resv_tmp.has_value()) { return true; }
                resv = resv_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage InsBasicConfig::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%u,%u,%u,%u", static_cast<uint8_t>(scenario),static_cast<uint8_t>(ahrsAiding),static_cast<uint8_t>(estBaseline),resv);
                return result;
            }
            
            bool operator==(const InsBasicConfig& lhs, const InsBasicConfig& rhs)
            {
                return 
                (
                (lhs.scenario == rhs.scenario)
                && (lhs.ahrsAiding == rhs.ahrsAiding)
                && (lhs.estBaseline == rhs.estBaseline)
                && (lhs.resv == rhs.resv)
                );
            }
            
            bool InsStateLla::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 15 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto yaw_tmp = StringUtils::fromString<float>(start, end);
                if(!yaw_tmp.has_value()) { return true; }
                yaw = yaw_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto pitch_tmp = StringUtils::fromString<float>(start, end);
                if(!pitch_tmp.has_value()) { return true; }
                pitch = pitch_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto roll_tmp = StringUtils::fromString<float>(start, end);
                if(!roll_tmp.has_value()) { return true; }
                roll = roll_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posLat_tmp = StringUtils::fromString<double>(start, end);
                if(!posLat_tmp.has_value()) { return true; }
                posLat = posLat_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posLon_tmp = StringUtils::fromString<double>(start, end);
                if(!posLon_tmp.has_value()) { return true; }
                posLon = posLon_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posAlt_tmp = StringUtils::fromString<double>(start, end);
                if(!posAlt_tmp.has_value()) { return true; }
                posAlt = posAlt_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velN_tmp = StringUtils::fromString<float>(start, end);
                if(!velN_tmp.has_value()) { return true; }
                velN = velN_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velE_tmp = StringUtils::fromString<float>(start, end);
                if(!velE_tmp.has_value()) { return true; }
                velE = velE_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velD_tmp = StringUtils::fromString<float>(start, end);
                if(!velD_tmp.has_value()) { return true; }
                velD = velD_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelX_tmp = StringUtils::fromString<float>(start, end);
                if(!accelX_tmp.has_value()) { return true; }
                accelX = accelX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelY_tmp = StringUtils::fromString<float>(start, end);
                if(!accelY_tmp.has_value()) { return true; }
                accelY = accelY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelZ_tmp = StringUtils::fromString<float>(start, end);
                if(!accelZ_tmp.has_value()) { return true; }
                accelZ = accelZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroX_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroX_tmp.has_value()) { return true; }
                gyroX = gyroX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroY_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroY_tmp.has_value()) { return true; }
                gyroY = gyroY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroZ_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroZ_tmp.has_value()) { return true; }
                gyroZ = gyroZ_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const InsStateLla& lhs, const InsStateLla& rhs)
            {
                return 
                (
                (lhs.yaw == rhs.yaw)
                && (lhs.pitch == rhs.pitch)
                && (lhs.roll == rhs.roll)
                && (lhs.posLat == rhs.posLat)
                && (lhs.posLon == rhs.posLon)
                && (lhs.posAlt == rhs.posAlt)
                && (lhs.velN == rhs.velN)
                && (lhs.velE == rhs.velE)
                && (lhs.velD == rhs.velD)
                && (lhs.accelX == rhs.accelX)
                && (lhs.accelY == rhs.accelY)
                && (lhs.accelZ == rhs.accelZ)
                && (lhs.gyroX == rhs.gyroX)
                && (lhs.gyroY == rhs.gyroY)
                && (lhs.gyroZ == rhs.gyroZ)
                );
            }
            
            bool InsStateEcef::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 15 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto yaw_tmp = StringUtils::fromString<float>(start, end);
                if(!yaw_tmp.has_value()) { return true; }
                yaw = yaw_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto pitch_tmp = StringUtils::fromString<float>(start, end);
                if(!pitch_tmp.has_value()) { return true; }
                pitch = pitch_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto roll_tmp = StringUtils::fromString<float>(start, end);
                if(!roll_tmp.has_value()) { return true; }
                roll = roll_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posEX_tmp = StringUtils::fromString<double>(start, end);
                if(!posEX_tmp.has_value()) { return true; }
                posEX = posEX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posEY_tmp = StringUtils::fromString<double>(start, end);
                if(!posEY_tmp.has_value()) { return true; }
                posEY = posEY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto posEZ_tmp = StringUtils::fromString<double>(start, end);
                if(!posEZ_tmp.has_value()) { return true; }
                posEZ = posEZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velEX_tmp = StringUtils::fromString<float>(start, end);
                if(!velEX_tmp.has_value()) { return true; }
                velEX = velEX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velEY_tmp = StringUtils::fromString<float>(start, end);
                if(!velEY_tmp.has_value()) { return true; }
                velEY = velEY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velEZ_tmp = StringUtils::fromString<float>(start, end);
                if(!velEZ_tmp.has_value()) { return true; }
                velEZ = velEZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelX_tmp = StringUtils::fromString<float>(start, end);
                if(!accelX_tmp.has_value()) { return true; }
                accelX = accelX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelY_tmp = StringUtils::fromString<float>(start, end);
                if(!accelY_tmp.has_value()) { return true; }
                accelY = accelY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelZ_tmp = StringUtils::fromString<float>(start, end);
                if(!accelZ_tmp.has_value()) { return true; }
                accelZ = accelZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroX_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroX_tmp.has_value()) { return true; }
                gyroX = gyroX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroY_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroY_tmp.has_value()) { return true; }
                gyroY = gyroY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroZ_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroZ_tmp.has_value()) { return true; }
                gyroZ = gyroZ_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const InsStateEcef& lhs, const InsStateEcef& rhs)
            {
                return 
                (
                (lhs.yaw == rhs.yaw)
                && (lhs.pitch == rhs.pitch)
                && (lhs.roll == rhs.roll)
                && (lhs.posEX == rhs.posEX)
                && (lhs.posEY == rhs.posEY)
                && (lhs.posEZ == rhs.posEZ)
                && (lhs.velEX == rhs.velEX)
                && (lhs.velEY == rhs.velEY)
                && (lhs.velEZ == rhs.velEZ)
                && (lhs.accelX == rhs.accelX)
                && (lhs.accelY == rhs.accelY)
                && (lhs.accelZ == rhs.accelZ)
                && (lhs.gyroX == rhs.gyroX)
                && (lhs.gyroY == rhs.gyroY)
                && (lhs.gyroZ == rhs.gyroZ)
                );
            }
            
            bool FilterStartupBias::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 7 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroBiasX_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroBiasX_tmp.has_value()) { return true; }
                gyroBiasX = gyroBiasX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroBiasY_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroBiasY_tmp.has_value()) { return true; }
                gyroBiasY = gyroBiasY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gyroBiasZ_tmp = StringUtils::fromString<float>(start, end);
                if(!gyroBiasZ_tmp.has_value()) { return true; }
                gyroBiasZ = gyroBiasZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelBiasX_tmp = StringUtils::fromString<float>(start, end);
                if(!accelBiasX_tmp.has_value()) { return true; }
                accelBiasX = accelBiasX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelBiasY_tmp = StringUtils::fromString<float>(start, end);
                if(!accelBiasY_tmp.has_value()) { return true; }
                accelBiasY = accelBiasY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto accelBiasZ_tmp = StringUtils::fromString<float>(start, end);
                if(!accelBiasZ_tmp.has_value()) { return true; }
                accelBiasZ = accelBiasZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto presBias_tmp = StringUtils::fromString<float>(start, end);
                if(!presBias_tmp.has_value()) { return true; }
                presBias = presBias_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage FilterStartupBias::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%f,%f,%f,%f,%f,%f,%f", gyroBiasX,gyroBiasY,gyroBiasZ,accelBiasX,accelBiasY,accelBiasZ,presBias);
                return result;
            }
            
            bool operator==(const FilterStartupBias& lhs, const FilterStartupBias& rhs)
            {
                return 
                (
                (lhs.gyroBiasX == rhs.gyroBiasX)
                && (lhs.gyroBiasY == rhs.gyroBiasY)
                && (lhs.gyroBiasZ == rhs.gyroBiasZ)
                && (lhs.accelBiasX == rhs.accelBiasX)
                && (lhs.accelBiasY == rhs.accelBiasY)
                && (lhs.accelBiasZ == rhs.accelBiasZ)
                && (lhs.presBias == rhs.presBias)
                );
            }
            
            bool InsRefOffset::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 6 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto refOffsetX_tmp = StringUtils::fromString<float>(start, end);
                if(!refOffsetX_tmp.has_value()) { return true; }
                refOffsetX = refOffsetX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto refOffsetY_tmp = StringUtils::fromString<float>(start, end);
                if(!refOffsetY_tmp.has_value()) { return true; }
                refOffsetY = refOffsetY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto refOffsetZ_tmp = StringUtils::fromString<float>(start, end);
                if(!refOffsetZ_tmp.has_value()) { return true; }
                refOffsetZ = refOffsetZ_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto refUncertX_tmp = StringUtils::fromString<float>(start, end);
                if(!refUncertX_tmp.has_value()) { return true; }
                refUncertX = refUncertX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto refUncertY_tmp = StringUtils::fromString<float>(start, end);
                if(!refUncertY_tmp.has_value()) { return true; }
                refUncertY = refUncertY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto refUncertZ_tmp = StringUtils::fromString<float>(start, end);
                if(!refUncertZ_tmp.has_value()) { return true; }
                refUncertZ = refUncertZ_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage InsRefOffset::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%f,%f,%f,%f,%f,%f", refOffsetX,refOffsetY,refOffsetZ,refUncertX,refUncertY,refUncertZ);
                return result;
            }
            
            bool operator==(const InsRefOffset& lhs, const InsRefOffset& rhs)
            {
                return 
                (
                (lhs.refOffsetX == rhs.refOffsetX)
                && (lhs.refOffsetY == rhs.refOffsetY)
                && (lhs.refOffsetZ == rhs.refOffsetZ)
                && (lhs.refUncertX == rhs.refUncertX)
                && (lhs.refUncertY == rhs.refUncertY)
                && (lhs.refUncertZ == rhs.refUncertZ)
                );
            }
            
            bool InsGnssSelect::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 6 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto activeReceiverSelect_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!activeReceiverSelect_tmp.has_value()) { return true; }
                activeReceiverSelect = static_cast<InsGnssSelect::ActiveReceiverSelect>(activeReceiverSelect_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto usedForNavTime_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!usedForNavTime_tmp.has_value()) { return true; }
                usedForNavTime = usedForNavTime_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto hysteresisTime_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!hysteresisTime_tmp.has_value()) { return true; }
                hysteresisTime = hysteresisTime_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto useGnssCompass_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!useGnssCompass_tmp.has_value()) { return true; }
                useGnssCompass = static_cast<InsGnssSelect::UseGnssCompass>(useGnssCompass_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto resv1_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!resv1_tmp.has_value()) { return true; }
                resv1 = resv1_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto resv2_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!resv2_tmp.has_value()) { return true; }
                resv2 = resv2_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage InsGnssSelect::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%u,%u,%u,%u,%u,%u", static_cast<uint8_t>(activeReceiverSelect),usedForNavTime,hysteresisTime,static_cast<uint8_t>(useGnssCompass),resv1,resv2);
                return result;
            }
            
            bool operator==(const InsGnssSelect& lhs, const InsGnssSelect& rhs)
            {
                return 
                (
                (lhs.activeReceiverSelect == rhs.activeReceiverSelect)
                && (lhs.usedForNavTime == rhs.usedForNavTime)
                && (lhs.hysteresisTime == rhs.hysteresisTime)
                && (lhs.useGnssCompass == rhs.useGnssCompass)
                && (lhs.resv1 == rhs.resv1)
                && (lhs.resv2 == rhs.resv2)
                );
            }
            
        } // namespace INS
        
        namespace System
        {
            bool UserTag::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 1 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                tag = AsciiMessage(start, end);
                
                return false; // no errors occurred
            }
            
            AsciiMessage UserTag::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%s", tag.c_str());
                return result;
            }
            
            bool operator==(const UserTag& lhs, const UserTag& rhs)
            {
                return 
                (
                (lhs.tag == rhs.tag)
                );
            }
            
            bool Model::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 1 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                model = AsciiMessage(start, end);
                
                return false; // no errors occurred
            }
            
            bool operator==(const Model& lhs, const Model& rhs)
            {
                return 
                (
                (lhs.model == rhs.model)
                );
            }
            
            bool HwVer::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 2 + 1)
                {
                    // This register may not have an optional field
                    if (tokens.size() != 1 + 1)
                    {
                        return true;
                    }
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto hwVer_tmp = StringUtils::fromString<uint32_t>(start, end);
                if(!hwVer_tmp.has_value()) { return true; }
                hwVer = hwVer_tmp.value();
                
                if(tokens.size() == (2 + 1))
                {
                    // This is an optional parameter.
                    start = tokens[index++] + 1;
                    end = tokens[index];
                    const auto hwMinVer_tmp = StringUtils::fromString<uint32_t>(start, end);
                    if(!hwMinVer_tmp.has_value()) { return true; }
                    hwMinVer = hwMinVer_tmp.value();
                }
                
                return false; // no errors occurred
            }
            
            bool operator==(const HwVer& lhs, const HwVer& rhs)
            {
                return 
                (
                (lhs.hwVer == rhs.hwVer)
                && (lhs.hwMinVer == rhs.hwMinVer)
                );
            }
            
            bool Serial::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 1 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto serialNum_tmp = StringUtils::fromString<uint32_t>(start, end);
                if(!serialNum_tmp.has_value()) { return true; }
                serialNum = serialNum_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const Serial& lhs, const Serial& rhs)
            {
                return 
                (
                (lhs.serialNum == rhs.serialNum)
                );
            }
            
            bool FwVer::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 1 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                fwVer = AsciiMessage(start, end);
                
                return false; // no errors occurred
            }
            
            bool operator==(const FwVer& lhs, const FwVer& rhs)
            {
                return 
                (
                (lhs.fwVer == rhs.fwVer)
                );
            }
            
            bool BaudRate::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 2 + 1)
                {
                    // This register may not have an optional field
                    if (tokens.size() != 1 + 1)
                    {
                        return true;
                    }
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto baudRate_tmp = StringUtils::fromString<uint32_t>(start, end);
                if(!baudRate_tmp.has_value()) { return true; }
                baudRate = static_cast<BaudRate::BaudRates>(baudRate_tmp.value());
                
                if(tokens.size() == (2 + 1))
                {
                    // This is an optional parameter.
                    start = tokens[index++] + 1;
                    end = tokens[index];
                    const auto serialPort_tmp = StringUtils::fromString<uint8_t>(start, end);
                    if(!serialPort_tmp.has_value()) { return true; }
                    serialPort = static_cast<BaudRate::SerialPort>(serialPort_tmp.value());
                }
                
                return false; // no errors occurred
            }
            
            AsciiMessage BaudRate::toString() const
            {
                AsciiMessage result = "";
                if (static_cast<uint8_t>(serialPort) == 0)
                {
                    std::snprintf(result.begin(), result.capacity(), "%d", static_cast<uint32_t>(baudRate));
                }
                else
                {
                    if (serialPort == SerialPort::Poll)
                    {
                        std::snprintf(result.begin(), result.capacity(), "%u,?", static_cast<uint32_t>(baudRate));
                    }
                    else
                    {
                        std::snprintf(result.begin(), result.capacity(), "%u,%u", static_cast<uint32_t>(baudRate),static_cast<uint8_t>(serialPort));
                    }
                }
                return result;
            }
            
            Command BaudRate::toReadCommand()
            {
                AsciiMessage commandString;
                if (serialPort == SerialPort::Poll) 
                {
                    std::snprintf(commandString.begin(), commandString.capacity(), "RRG,%02d,?", _id);
                }
                else 
                {
                    std::snprintf(commandString.begin(), commandString.capacity(), "RRG,%02d,%1d", _id, static_cast<uint8_t>(serialPort));
                }
                
                Command readCommand(commandString, 6);
                return readCommand;
            }
            
            bool operator==(const BaudRate& lhs, const BaudRate& rhs)
            {
                return 
                (
                (lhs.baudRate == rhs.baudRate)
                && (lhs.serialPort == rhs.serialPort)
                );
            }
            
            bool AsyncOutputType::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 2 + 1)
                {
                    // This register may not have an optional field
                    if (tokens.size() != 1 + 1)
                    {
                        return true;
                    }
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto ador_tmp = StringUtils::fromString<uint32_t>(start, end);
                if(!ador_tmp.has_value()) { return true; }
                ador = static_cast<AsyncOutputType::Ador>(ador_tmp.value());
                
                if(tokens.size() == (2 + 1))
                {
                    // This is an optional parameter.
                    start = tokens[index++] + 1;
                    end = tokens[index];
                    const auto serialPort_tmp = StringUtils::fromString<uint8_t>(start, end);
                    if(!serialPort_tmp.has_value()) { return true; }
                    serialPort = static_cast<AsyncOutputType::SerialPort>(serialPort_tmp.value());
                }
                
                return false; // no errors occurred
            }
            
            AsciiMessage AsyncOutputType::toString() const
            {
                AsciiMessage result = "";
                if (static_cast<uint8_t>(serialPort) == 0)
                {
                    std::snprintf(result.begin(), result.capacity(), "%d", static_cast<uint32_t>(ador));
                }
                else
                {
                    if (serialPort == SerialPort::Poll)
                    {
                        std::snprintf(result.begin(), result.capacity(), "%u,?", static_cast<uint32_t>(ador));
                    }
                    else
                    {
                        std::snprintf(result.begin(), result.capacity(), "%u,%u", static_cast<uint32_t>(ador),static_cast<uint8_t>(serialPort));
                    }
                }
                return result;
            }
            
            Command AsyncOutputType::toReadCommand()
            {
                AsciiMessage commandString;
                if (serialPort == SerialPort::Poll) 
                {
                    std::snprintf(commandString.begin(), commandString.capacity(), "RRG,%02d,?", _id);
                }
                else 
                {
                    std::snprintf(commandString.begin(), commandString.capacity(), "RRG,%02d,%1d", _id, static_cast<uint8_t>(serialPort));
                }
                
                Command readCommand(commandString, 6);
                return readCommand;
            }
            
            bool operator==(const AsyncOutputType& lhs, const AsyncOutputType& rhs)
            {
                return 
                (
                (lhs.ador == rhs.ador)
                && (lhs.serialPort == rhs.serialPort)
                );
            }
            
            bool AsyncOutputFreq::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 2 + 1)
                {
                    // This register may not have an optional field
                    if (tokens.size() != 1 + 1)
                    {
                        return true;
                    }
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto adof_tmp = StringUtils::fromString<uint32_t>(start, end);
                if(!adof_tmp.has_value()) { return true; }
                adof = static_cast<AsyncOutputFreq::Adof>(adof_tmp.value());
                
                if(tokens.size() == (2 + 1))
                {
                    // This is an optional parameter.
                    start = tokens[index++] + 1;
                    end = tokens[index];
                    const auto serialPort_tmp = StringUtils::fromString<uint8_t>(start, end);
                    if(!serialPort_tmp.has_value()) { return true; }
                    serialPort = static_cast<AsyncOutputFreq::SerialPort>(serialPort_tmp.value());
                }
                
                return false; // no errors occurred
            }
            
            AsciiMessage AsyncOutputFreq::toString() const
            {
                AsciiMessage result = "";
                if (static_cast<uint8_t>(serialPort) == 0)
                {
                    std::snprintf(result.begin(), result.capacity(), "%d", static_cast<uint32_t>(adof));
                }
                else
                {
                    if (serialPort == SerialPort::Poll)
                    {
                        std::snprintf(result.begin(), result.capacity(), "%u,?", static_cast<uint32_t>(adof));
                    }
                    else
                    {
                        std::snprintf(result.begin(), result.capacity(), "%u,%u", static_cast<uint32_t>(adof),static_cast<uint8_t>(serialPort));
                    }
                }
                return result;
            }
            
            Command AsyncOutputFreq::toReadCommand()
            {
                AsciiMessage commandString;
                if (serialPort == SerialPort::Poll) 
                {
                    std::snprintf(commandString.begin(), commandString.capacity(), "RRG,%02d,?", _id);
                }
                else 
                {
                    std::snprintf(commandString.begin(), commandString.capacity(), "RRG,%02d,%1d", _id, static_cast<uint8_t>(serialPort));
                }
                
                Command readCommand(commandString, 6);
                return readCommand;
            }
            
            bool operator==(const AsyncOutputFreq& lhs, const AsyncOutputFreq& rhs)
            {
                return 
                (
                (lhs.adof == rhs.adof)
                && (lhs.serialPort == rhs.serialPort)
                );
            }
            
            bool ProtocolControl::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 7 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto asciiAppendCount_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!asciiAppendCount_tmp.has_value()) { return true; }
                asciiAppendCount = static_cast<ProtocolControl::AsciiAppendCount>(asciiAppendCount_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto asciiAppendStatus_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!asciiAppendStatus_tmp.has_value()) { return true; }
                asciiAppendStatus = static_cast<ProtocolControl::AsciiAppendStatus>(asciiAppendStatus_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto spiAppendCount_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!spiAppendCount_tmp.has_value()) { return true; }
                spiAppendCount = static_cast<ProtocolControl::SpiAppendCount>(spiAppendCount_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto spiAppendStatus_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!spiAppendStatus_tmp.has_value()) { return true; }
                spiAppendStatus = static_cast<ProtocolControl::SpiAppendStatus>(spiAppendStatus_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto asciiChecksum_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!asciiChecksum_tmp.has_value()) { return true; }
                asciiChecksum = static_cast<ProtocolControl::AsciiChecksum>(asciiChecksum_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto spiChecksum_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!spiChecksum_tmp.has_value()) { return true; }
                spiChecksum = static_cast<ProtocolControl::SpiChecksum>(spiChecksum_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto errorMode_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!errorMode_tmp.has_value()) { return true; }
                errorMode = static_cast<ProtocolControl::ErrorMode>(errorMode_tmp.value());
                
                return false; // no errors occurred
            }
            
            AsciiMessage ProtocolControl::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%u,%u,%u,%u,%u,%u,%u", static_cast<uint8_t>(asciiAppendCount),static_cast<uint8_t>(asciiAppendStatus),static_cast<uint8_t>(spiAppendCount),static_cast<uint8_t>(spiAppendStatus),static_cast<uint8_t>(asciiChecksum),static_cast<uint8_t>(spiChecksum),static_cast<uint8_t>(errorMode));
                return result;
            }
            
            bool operator==(const ProtocolControl& lhs, const ProtocolControl& rhs)
            {
                return 
                (
                (lhs.asciiAppendCount == rhs.asciiAppendCount)
                && (lhs.asciiAppendStatus == rhs.asciiAppendStatus)
                && (lhs.spiAppendCount == rhs.spiAppendCount)
                && (lhs.spiAppendStatus == rhs.spiAppendStatus)
                && (lhs.asciiChecksum == rhs.asciiChecksum)
                && (lhs.spiChecksum == rhs.spiChecksum)
                && (lhs.errorMode == rhs.errorMode)
                );
            }
            
            bool SyncControl::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 9 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto syncInMode_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!syncInMode_tmp.has_value()) { return true; }
                syncInMode = static_cast<SyncControl::SyncInMode>(syncInMode_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto syncInEdge_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!syncInEdge_tmp.has_value()) { return true; }
                syncInEdge = static_cast<SyncControl::SyncInEdge>(syncInEdge_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto syncInSkipFactor_tmp = StringUtils::fromString<uint16_t>(start, end);
                if(!syncInSkipFactor_tmp.has_value()) { return true; }
                syncInSkipFactor = syncInSkipFactor_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto resv1_tmp = StringUtils::fromString<uint32_t>(start, end);
                if(!resv1_tmp.has_value()) { return true; }
                resv1 = resv1_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto syncOutMode_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!syncOutMode_tmp.has_value()) { return true; }
                syncOutMode = static_cast<SyncControl::SyncOutMode>(syncOutMode_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto syncOutPolarity_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!syncOutPolarity_tmp.has_value()) { return true; }
                syncOutPolarity = static_cast<SyncControl::SyncOutPolarity>(syncOutPolarity_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto syncOutSkipFactor_tmp = StringUtils::fromString<uint16_t>(start, end);
                if(!syncOutSkipFactor_tmp.has_value()) { return true; }
                syncOutSkipFactor = syncOutSkipFactor_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto syncOutPulseWidth_tmp = StringUtils::fromString<uint32_t>(start, end);
                if(!syncOutPulseWidth_tmp.has_value()) { return true; }
                syncOutPulseWidth = syncOutPulseWidth_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto resv2_tmp = StringUtils::fromString<uint32_t>(start, end);
                if(!resv2_tmp.has_value()) { return true; }
                resv2 = resv2_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage SyncControl::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%u,%u,%u,%u,%u,%u,%u,%u,%u", static_cast<uint8_t>(syncInMode),static_cast<uint8_t>(syncInEdge),syncInSkipFactor,resv1,static_cast<uint8_t>(syncOutMode),static_cast<uint8_t>(syncOutPolarity),syncOutSkipFactor,syncOutPulseWidth,resv2);
                return result;
            }
            
            bool operator==(const SyncControl& lhs, const SyncControl& rhs)
            {
                return 
                (
                (lhs.syncInMode == rhs.syncInMode)
                && (lhs.syncInEdge == rhs.syncInEdge)
                && (lhs.syncInSkipFactor == rhs.syncInSkipFactor)
                && (lhs.resv1 == rhs.resv1)
                && (lhs.syncOutMode == rhs.syncOutMode)
                && (lhs.syncOutPolarity == rhs.syncOutPolarity)
                && (lhs.syncOutSkipFactor == rhs.syncOutSkipFactor)
                && (lhs.syncOutPulseWidth == rhs.syncOutPulseWidth)
                && (lhs.resv2 == rhs.resv2)
                );
            }
            
            bool SyncStatus::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 3 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto syncInCount_tmp = StringUtils::fromString<uint32_t>(start, end);
                if(!syncInCount_tmp.has_value()) { return true; }
                syncInCount = syncInCount_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto syncInTime_tmp = StringUtils::fromString<uint32_t>(start, end);
                if(!syncInTime_tmp.has_value()) { return true; }
                syncInTime = syncInTime_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto syncOutCount_tmp = StringUtils::fromString<uint32_t>(start, end);
                if(!syncOutCount_tmp.has_value()) { return true; }
                syncOutCount = syncOutCount_tmp.value();
                
                return false; // no errors occurred
            }
            
            bool operator==(const SyncStatus& lhs, const SyncStatus& rhs)
            {
                return 
                (
                (lhs.syncInCount == rhs.syncInCount)
                && (lhs.syncInTime == rhs.syncInTime)
                && (lhs.syncOutCount == rhs.syncOutCount)
                );
            }
            
            bool BinaryOutput::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                std::optional<uint16_t> tmpRet = StringUtils::fromStringHex<uint16_t>(start, end);
                if (!tmpRet) { return true; }
                asyncMode = *tmpRet;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                tmpRet = StringUtils::fromString<uint16_t>(start, end);
                if (!tmpRet) { return true; }
                rateDivisor = *tmpRet;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                tmpRet = StringUtils::fromStringHex<uint8_t>(start, end);
                if (!tmpRet) { return true; }
                uint16_t group = *tmpRet;
                
                if (group & 0x80)
                {
                    start = tokens[index++] + 1;
                    end = tokens[index];
                    tmpRet = StringUtils::fromStringHex<uint8_t>(start, end);
                    if (!tmpRet) { return true; }
                    group |= *tmpRet << 8;
                }
                
                if (group & COMMON_BIT)
                {
                    start = tokens[index++] + 1;
                    end = tokens[index];
                    uint32_t common_ret = 0;
                    auto common_type = StringUtils::fromStringHex<uint16_t>(start, end);
                    if (!common_type.has_value()) { return true; }
                    common_ret = common_type.value();
                    if (common_ret & 0x8000)
                    {
                        start = tokens[index++] + 1;
                        end = tokens[index];
                        auto common_type_ext = StringUtils::fromStringHex<uint16_t>(start, end);
                        if (!common_type_ext.has_value()) { return true; }
                        common_ret |= common_type_ext.value() << 16;
                    }
                    this->common = common_ret;
                }
                
                if (group & TIME_BIT)
                {
                    start = tokens[index++] + 1;
                    end = tokens[index];
                    uint32_t time_ret = 0;
                    auto time_type = StringUtils::fromStringHex<uint16_t>(start, end);
                    if (!time_type.has_value()) { return true; }
                    time_ret = time_type.value();
                    if (time_ret & 0x8000)
                    {
                        start = tokens[index++] + 1;
                        end = tokens[index];
                        auto time_type_ext = StringUtils::fromStringHex<uint16_t>(start, end);
                        if (!time_type_ext.has_value()) { return true; }
                        time_ret |= time_type_ext.value() << 16;
                    }
                    this->time = time_ret;
                }
                
                if (group & IMU_BIT)
                {
                    start = tokens[index++] + 1;
                    end = tokens[index];
                    uint32_t imu_ret = 0;
                    auto imu_type = StringUtils::fromStringHex<uint16_t>(start, end);
                    if (!imu_type.has_value()) { return true; }
                    imu_ret = imu_type.value();
                    if (imu_ret & 0x8000)
                    {
                        start = tokens[index++] + 1;
                        end = tokens[index];
                        auto imu_type_ext = StringUtils::fromStringHex<uint16_t>(start, end);
                        if (!imu_type_ext.has_value()) { return true; }
                        imu_ret |= imu_type_ext.value() << 16;
                    }
                    this->imu = imu_ret;
                }
                
                if (group & GNSS_BIT)
                {
                    start = tokens[index++] + 1;
                    end = tokens[index];
                    uint32_t gnss_ret = 0;
                    auto gnss_type = StringUtils::fromStringHex<uint16_t>(start, end);
                    if (!gnss_type.has_value()) { return true; }
                    gnss_ret = gnss_type.value();
                    if (gnss_ret & 0x8000)
                    {
                        start = tokens[index++] + 1;
                        end = tokens[index];
                        auto gnss_type_ext = StringUtils::fromStringHex<uint16_t>(start, end);
                        if (!gnss_type_ext.has_value()) { return true; }
                        gnss_ret |= gnss_type_ext.value() << 16;
                    }
                    this->gnss = gnss_ret;
                }
                
                if (group & ATTITUDE_BIT)
                {
                    start = tokens[index++] + 1;
                    end = tokens[index];
                    uint32_t attitude_ret = 0;
                    auto attitude_type = StringUtils::fromStringHex<uint16_t>(start, end);
                    if (!attitude_type.has_value()) { return true; }
                    attitude_ret = attitude_type.value();
                    if (attitude_ret & 0x8000)
                    {
                        start = tokens[index++] + 1;
                        end = tokens[index];
                        auto attitude_type_ext = StringUtils::fromStringHex<uint16_t>(start, end);
                        if (!attitude_type_ext.has_value()) { return true; }
                        attitude_ret |= attitude_type_ext.value() << 16;
                    }
                    this->attitude = attitude_ret;
                }
                
                if (group & INS_BIT)
                {
                    start = tokens[index++] + 1;
                    end = tokens[index];
                    uint32_t ins_ret = 0;
                    auto ins_type = StringUtils::fromStringHex<uint16_t>(start, end);
                    if (!ins_type.has_value()) { return true; }
                    ins_ret = ins_type.value();
                    if (ins_ret & 0x8000)
                    {
                        start = tokens[index++] + 1;
                        end = tokens[index];
                        auto ins_type_ext = StringUtils::fromStringHex<uint16_t>(start, end);
                        if (!ins_type_ext.has_value()) { return true; }
                        ins_ret |= ins_type_ext.value() << 16;
                    }
                    this->ins = ins_ret;
                }
                
                if (group & GNSS2_BIT)
                {
                    start = tokens[index++] + 1;
                    end = tokens[index];
                    uint32_t gnss2_ret = 0;
                    auto gnss2_type = StringUtils::fromStringHex<uint16_t>(start, end);
                    if (!gnss2_type.has_value()) { return true; }
                    gnss2_ret = gnss2_type.value();
                    if (gnss2_ret & 0x8000)
                    {
                        start = tokens[index++] + 1;
                        end = tokens[index];
                        auto gnss2_type_ext = StringUtils::fromStringHex<uint16_t>(start, end);
                        if (!gnss2_type_ext.has_value()) { return true; }
                        gnss2_ret |= gnss2_type_ext.value() << 16;
                    }
                    this->gnss2 = gnss2_ret;
                }
                
                if (group & GNSS3_BIT)
                {
                    start = tokens[index++] + 1;
                    end = tokens[index];
                    uint32_t gnss3_ret = 0;
                    auto gnss3_type = StringUtils::fromStringHex<uint16_t>(start, end);
                    if (!gnss3_type.has_value()) { return true; }
                    gnss3_ret = gnss3_type.value();
                    if (gnss3_ret & 0x8000)
                    {
                        start = tokens[index++] + 1;
                        end = tokens[index];
                        auto gnss3_type_ext = StringUtils::fromStringHex<uint16_t>(start, end);
                        if (!gnss3_type_ext.has_value()) { return true; }
                        gnss3_ret |= gnss3_type_ext.value() << 16;
                    }
                    this->gnss3 = gnss3_ret;
                }
                
                return false; // no errors occurred
            }
            
            AsciiMessage BinaryOutput::toString() const
            {
                AsciiMessage result = "";
                BinaryHeader binaryHeader = toBinaryHeader();
                std::snprintf(result.end(), result.numAvailable(), "%1X,%d%s", uint16_t(asyncMode), rateDivisor, binaryHeaderToString<AsciiMessage::capacity()>(binaryHeader).c_str());
                return result;
            }
            
            BinaryHeader BinaryOutputMeasurements::toBinaryHeader() const noexcept
            {
                BinaryHeader result;
                uint8_t group = 0;
                uint8_t groupExt = 0;
                if (uint32_t(this->common) > 0) group += COMMON_BIT;
                if (uint32_t(this->time) > 0) group += TIME_BIT;
                if (uint32_t(this->imu) > 0) group += IMU_BIT;
                if (uint32_t(this->gnss) > 0) group += GNSS_BIT;
                if (uint32_t(this->attitude) > 0) group += ATTITUDE_BIT;
                if (uint32_t(this->ins) > 0) group += INS_BIT;
                if (uint32_t(this->gnss2) > 0) group += GNSS2_BIT;
                if (uint32_t(this->gnss3) > 0) groupExt += (GNSS3_BIT >> 8);
                if (uint32_t(this->gnss3) > 0) group |= 0x80;
                result.outputGroups.push_back(group);
                if (groupExt > 0) result.outputGroups.push_back(groupExt);
                
                if (uint32_t(this->common) > 0)
                {
                    result.outputTypes.push_back(uint32_t(this->common));
                    if ((uint32_t(this->common) & 0xFFFF0000) > 0)
                    {
                        result.outputTypes.back() |= 0x8000;  // Set extension bit
                        result.outputTypes.push_back((uint32_t(this->common) & 0xFFFF0000) >> 16);
                    }
                }
                
                if (uint32_t(this->time) > 0)
                {
                    result.outputTypes.push_back(uint32_t(this->time));
                    if ((uint32_t(this->time) & 0xFFFF0000) > 0)
                    {
                        result.outputTypes.back() |= 0x8000;  // Set extension bit
                        result.outputTypes.push_back((uint32_t(this->time) & 0xFFFF0000) >> 16);
                    }
                }
                
                if (uint32_t(this->imu) > 0)
                {
                    result.outputTypes.push_back(uint32_t(this->imu));
                    if ((uint32_t(this->imu) & 0xFFFF0000) > 0)
                    {
                        result.outputTypes.back() |= 0x8000;  // Set extension bit
                        result.outputTypes.push_back((uint32_t(this->imu) & 0xFFFF0000) >> 16);
                    }
                }
                
                if (uint32_t(this->gnss) > 0)
                {
                    result.outputTypes.push_back(uint32_t(this->gnss));
                    if ((uint32_t(this->gnss) & 0xFFFF0000) > 0)
                    {
                        result.outputTypes.back() |= 0x8000;  // Set extension bit
                        result.outputTypes.push_back((uint32_t(this->gnss) & 0xFFFF0000) >> 16);
                    }
                }
                
                if (uint32_t(this->attitude) > 0)
                {
                    result.outputTypes.push_back(uint32_t(this->attitude));
                    if ((uint32_t(this->attitude) & 0xFFFF0000) > 0)
                    {
                        result.outputTypes.back() |= 0x8000;  // Set extension bit
                        result.outputTypes.push_back((uint32_t(this->attitude) & 0xFFFF0000) >> 16);
                    }
                }
                
                if (uint32_t(this->ins) > 0)
                {
                    result.outputTypes.push_back(uint32_t(this->ins));
                    if ((uint32_t(this->ins) & 0xFFFF0000) > 0)
                    {
                        result.outputTypes.back() |= 0x8000;  // Set extension bit
                        result.outputTypes.push_back((uint32_t(this->ins) & 0xFFFF0000) >> 16);
                    }
                }
                
                if (uint32_t(this->gnss2) > 0)
                {
                    result.outputTypes.push_back(uint32_t(this->gnss2));
                    if ((uint32_t(this->gnss2) & 0xFFFF0000) > 0)
                    {
                        result.outputTypes.back() |= 0x8000;  // Set extension bit
                        result.outputTypes.push_back((uint32_t(this->gnss2) & 0xFFFF0000) >> 16);
                    }
                }
                
                if (uint32_t(this->gnss3) > 0)
                {
                    result.outputTypes.push_back(uint32_t(this->gnss3));
                    if ((uint32_t(this->gnss3) & 0xFFFF0000) > 0)
                    {
                        result.outputTypes.back() |= 0x8000;  // Set extension bit
                        result.outputTypes.push_back((uint32_t(this->gnss3) & 0xFFFF0000) >> 16);
                    }
                }
                
                return result;
            }
            
            bool NmeaOutput1::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 5 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto port_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!port_tmp.has_value()) { return true; }
                port = static_cast<NmeaOutput1::Port>(port_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto rate_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!rate_tmp.has_value()) { return true; }
                rate = static_cast<NmeaOutput1::Rate>(rate_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto mode_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!mode_tmp.has_value()) { return true; }
                mode = static_cast<NmeaOutput1::Mode>(mode_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gnssSelect_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!gnssSelect_tmp.has_value()) { return true; }
                gnssSelect = static_cast<NmeaOutput1::GnssSelect>(gnssSelect_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto msgSelection_tmp = StringUtils::fromStringHex<uint32_t>(start, end);
                if(!msgSelection_tmp.has_value()) { return true; }
                msgSelection = msgSelection_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage NmeaOutput1::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%u,%u,%u,%u,%X", static_cast<uint8_t>(port),static_cast<uint8_t>(rate),static_cast<uint8_t>(mode),static_cast<uint8_t>(gnssSelect),uint32_t(msgSelection));
                return result;
            }
            
            bool operator==(const NmeaOutput1& lhs, const NmeaOutput1& rhs)
            {
                return 
                (
                (lhs.port == rhs.port)
                && (lhs.rate == rhs.rate)
                && (lhs.mode == rhs.mode)
                && (lhs.gnssSelect == rhs.gnssSelect)
                && (uint32_t(lhs.msgSelection) == uint32_t(rhs.msgSelection))
                );
            }
            
            bool NmeaOutput2::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 5 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto port_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!port_tmp.has_value()) { return true; }
                port = static_cast<NmeaOutput2::Port>(port_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto rate_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!rate_tmp.has_value()) { return true; }
                rate = static_cast<NmeaOutput2::Rate>(rate_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto mode_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!mode_tmp.has_value()) { return true; }
                mode = static_cast<NmeaOutput2::Mode>(mode_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gnssSelect_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!gnssSelect_tmp.has_value()) { return true; }
                gnssSelect = static_cast<NmeaOutput2::GnssSelect>(gnssSelect_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto msgSelection_tmp = StringUtils::fromStringHex<uint32_t>(start, end);
                if(!msgSelection_tmp.has_value()) { return true; }
                msgSelection = msgSelection_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage NmeaOutput2::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%u,%u,%u,%u,%X", static_cast<uint8_t>(port),static_cast<uint8_t>(rate),static_cast<uint8_t>(mode),static_cast<uint8_t>(gnssSelect),uint32_t(msgSelection));
                return result;
            }
            
            bool operator==(const NmeaOutput2& lhs, const NmeaOutput2& rhs)
            {
                return 
                (
                (lhs.port == rhs.port)
                && (lhs.rate == rhs.rate)
                && (lhs.mode == rhs.mode)
                && (lhs.gnssSelect == rhs.gnssSelect)
                && (uint32_t(lhs.msgSelection) == uint32_t(rhs.msgSelection))
                );
            }
            
            bool LegacyCompatibilitySettings::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 4 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto insLegacy_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!insLegacy_tmp.has_value()) { return true; }
                insLegacy = static_cast<LegacyCompatibilitySettings::InsLegacy>(insLegacy_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto gnssLegacy_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!gnssLegacy_tmp.has_value()) { return true; }
                gnssLegacy = gnssLegacy_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto imuLegacy_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!imuLegacy_tmp.has_value()) { return true; }
                imuLegacy = static_cast<LegacyCompatibilitySettings::ImuLegacy>(imuLegacy_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto hwLegacy_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!hwLegacy_tmp.has_value()) { return true; }
                hwLegacy = static_cast<LegacyCompatibilitySettings::HwLegacy>(hwLegacy_tmp.value());
                
                return false; // no errors occurred
            }
            
            AsciiMessage LegacyCompatibilitySettings::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%u,%u,%u,%u", static_cast<uint8_t>(insLegacy),uint8_t(gnssLegacy),static_cast<uint8_t>(imuLegacy),static_cast<uint8_t>(hwLegacy));
                return result;
            }
            
            bool operator==(const LegacyCompatibilitySettings& lhs, const LegacyCompatibilitySettings& rhs)
            {
                return 
                (
                (lhs.insLegacy == rhs.insLegacy)
                && (uint8_t(lhs.gnssLegacy) == uint8_t(rhs.gnssLegacy))
                && (lhs.imuLegacy == rhs.imuLegacy)
                && (lhs.hwLegacy == rhs.hwLegacy)
                );
            }
            
        } // namespace System
        
        namespace VelocityAiding
        {
            bool VelAidingMeas::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 3 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velocityX_tmp = StringUtils::fromString<float>(start, end);
                if(!velocityX_tmp.has_value()) { return true; }
                velocityX = velocityX_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velocityY_tmp = StringUtils::fromString<float>(start, end);
                if(!velocityY_tmp.has_value()) { return true; }
                velocityY = velocityY_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velocityZ_tmp = StringUtils::fromString<float>(start, end);
                if(!velocityZ_tmp.has_value()) { return true; }
                velocityZ = velocityZ_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage VelAidingMeas::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%f,%f,%f", velocityX,velocityY,velocityZ);
                return result;
            }
            
            bool operator==(const VelAidingMeas& lhs, const VelAidingMeas& rhs)
            {
                return 
                (
                (lhs.velocityX == rhs.velocityX)
                && (lhs.velocityY == rhs.velocityY)
                && (lhs.velocityZ == rhs.velocityZ)
                );
            }
            
            bool VelAidingControl::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 3 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velAidEnable_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!velAidEnable_tmp.has_value()) { return true; }
                velAidEnable = static_cast<VelAidingControl::VelAidEnable>(velAidEnable_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto velUncertTuning_tmp = StringUtils::fromString<float>(start, end);
                if(!velUncertTuning_tmp.has_value()) { return true; }
                velUncertTuning = velUncertTuning_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto resv_tmp = StringUtils::fromString<float>(start, end);
                if(!resv_tmp.has_value()) { return true; }
                resv = resv_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage VelAidingControl::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%u,%f,%f", static_cast<uint8_t>(velAidEnable),velUncertTuning,resv);
                return result;
            }
            
            bool operator==(const VelAidingControl& lhs, const VelAidingControl& rhs)
            {
                return 
                (
                (lhs.velAidEnable == rhs.velAidEnable)
                && (lhs.velUncertTuning == rhs.velUncertTuning)
                && (lhs.resv == rhs.resv)
                );
            }
            
        } // namespace VelocityAiding
        
        namespace WorldMagGravityModel
        {
            bool RefModelConfig::fromString(const AsciiMessage& response)
            {
                const auto tokens = findIndexOfFieldSeparators(response);
                if (tokens.size() != 9 + 1)
                {
                    return true;
                }
                
                int index = 0;
                const char* start;
                const char* end;
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto enableMagModel_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!enableMagModel_tmp.has_value()) { return true; }
                enableMagModel = static_cast<RefModelConfig::EnableMagModel>(enableMagModel_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto enableGravityModel_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!enableGravityModel_tmp.has_value()) { return true; }
                enableGravityModel = static_cast<RefModelConfig::EnableGravityModel>(enableGravityModel_tmp.value());
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto resv1_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!resv1_tmp.has_value()) { return true; }
                resv1 = resv1_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto resv2_tmp = StringUtils::fromString<uint8_t>(start, end);
                if(!resv2_tmp.has_value()) { return true; }
                resv2 = resv2_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto recalcThreshold_tmp = StringUtils::fromString<uint32_t>(start, end);
                if(!recalcThreshold_tmp.has_value()) { return true; }
                recalcThreshold = recalcThreshold_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto year_tmp = StringUtils::fromString<float>(start, end);
                if(!year_tmp.has_value()) { return true; }
                year = year_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto latitude_tmp = StringUtils::fromString<double>(start, end);
                if(!latitude_tmp.has_value()) { return true; }
                latitude = latitude_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto longitude_tmp = StringUtils::fromString<double>(start, end);
                if(!longitude_tmp.has_value()) { return true; }
                longitude = longitude_tmp.value();
                
                start = tokens[index++] + 1;
                end = tokens[index];
                const auto altitude_tmp = StringUtils::fromString<double>(start, end);
                if(!altitude_tmp.has_value()) { return true; }
                altitude = altitude_tmp.value();
                
                return false; // no errors occurred
            }
            
            AsciiMessage RefModelConfig::toString() const
            {
                AsciiMessage result = "";
                std::snprintf(result.begin(), result.capacity(), "%u,%u,%u,%u,%u,%f,%f,%f,%f", static_cast<uint8_t>(enableMagModel),static_cast<uint8_t>(enableGravityModel),resv1,resv2,recalcThreshold,year,latitude,longitude,altitude);
                return result;
            }
            
            bool operator==(const RefModelConfig& lhs, const RefModelConfig& rhs)
            {
                return 
                (
                (lhs.enableMagModel == rhs.enableMagModel)
                && (lhs.enableGravityModel == rhs.enableGravityModel)
                && (lhs.resv1 == rhs.resv1)
                && (lhs.resv2 == rhs.resv2)
                && (lhs.recalcThreshold == rhs.recalcThreshold)
                && (lhs.year == rhs.year)
                && (lhs.latitude == rhs.latitude)
                && (lhs.longitude == rhs.longitude)
                && (lhs.altitude == rhs.altitude)
                );
            }
            
        } // namespace WorldMagGravityModel
        
        Vector<const char*, Config::PacketFinders::asciiMaxFieldCount> findIndexOfFieldSeparators(const AsciiMessage& input)
        {
            Vector<const char*, Config::PacketFinders::asciiMaxFieldCount> indexes;
            size_t offset = 7;
            if (input[0] != '$') 
            {
                indexes.push_back(input.begin() - 1);
                offset = 0;
            }
            
            for (size_t i = input.find(',', offset); i < input.length(); i++) 
            {
                if (input[i] == ',' || input[i] == '*') 
                { 
                    indexes.push_back(&input[i]); 
                }
            }
            
            if (input[0] != '$') 
            { 
                indexes.push_back(input.end()); 
            }
            
            return indexes;
        }
        
        
    } // namespace Registers
} // namespace VN

