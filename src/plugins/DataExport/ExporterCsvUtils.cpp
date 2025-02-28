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

#include "Exporter.hpp"
#include "ExporterCsvUtils.hpp"
#include "Implementation/FaPacketProtocol.hpp"
#include "Implementation/AsciiPacketProtocol.hpp"

namespace VN
{

std::array<std::array<CsvTypeInfo, 19>, 7> dataTypes{{
    {CsvTypeInfo{U64, 1},
     {U64, 1},
     {U64, 1},
     {FLO, 3},
     {FLO, 4},
     {FLO, 3},
     {DUB, 3},
     {FLO, 3},
     {FLO, 3},
     {FLO, 6},
     {FLO, 5},
     {FLO, 7},
     {U16, 1},
     {U32, 1},
     {U64, 1},
     {NON, 0},
     {NON, 0},
     {NON, 0},
     {NON, 0}},  // COMMON
    {CsvTypeInfo{U64, 1},
     {U64, 1},
     {U64, 1},
     {U16, 1},
     {U64, 1},
     {U64, 1},
     {UTC, 1},
     {U32, 1},
     {U32, 1},
     {U8, 1},
     {NON, 0},
     {NON, 0},
     {NON, 0},
     {NON, 0},
     {NON, 0},
     {NON, 0},
     {NON, 0},
     {NON, 0},
     {NON, 0}},  // TIME
    {CsvTypeInfo{U16, 1},
     {FLO, 3},
     {FLO, 3},
     {FLO, 3},
     {FLO, 1},
     {FLO, 1},
     {FLO, 4},
     {FLO, 3},
     {FLO, 3},
     {FLO, 3},
     {FLO, 3},
     {U16, 1},
     {NON, 0},
     {NON, 0},
     {NON, 0},
     {NON, 0},
     {NON, 0},
     {NON, 0},
     {NON, 0}},  // IMU
    {CsvTypeInfo{UTC, 1},
     {U64, 1},
     {U16, 1},
     {U8, 1},
     {U8, 1},
     {DUB, 3},
     {DUB, 3},
     {FLO, 3},
     {FLO, 3},
     {FLO, 3},
     {FLO, 1},
     {FLO, 1},
     {U16, 1},
     {FLO, 7},
     {SAT, 1},
     {NON, 0},
     {RAW, 1},
     {U16, 1},
     {DUB, 1}},  // GNSS
    {CsvTypeInfo{U16, 1},
     {FLO, 3},
     {FLO, 4},
     {FLO, 9},
     {FLO, 3},
     {FLO, 3},
     {FLO, 3},
     {FLO, 3},
     {FLO, 3},
     {UNK, 12},
     {NON, 0},
     {NON, 0},
     {FLO, 3},
     {FLO, 1},
     {NON, 0},
     {NON, 0},
     {NON, 0},
     {NON, 0},
     {NON, 0}},  // ATTITUDE
    {CsvTypeInfo{U16, 1},
     {DUB, 3},
     {DUB, 3},
     {FLO, 3},
     {FLO, 3},
     {FLO, 3},
     {FLO, 3},
     {FLO, 3},
     {FLO, 3},
     {FLO, 1},
     {FLO, 1},
     {NON, 0},
     {NON, 0},
     {NON, 0},
     {NON, 0},
     {NON, 0},
     {NON, 0},
     {NON, 0},
     {NON, 0}},  // INS
    {CsvTypeInfo{UTC, 1},
     {U64, 1},
     {U16, 1},
     {U8, 1},
     {U8, 1},
     {DUB, 3},
     {DUB, 3},
     {FLO, 3},
     {FLO, 3},
     {FLO, 3},
     {FLO, 1},
     {FLO, 1},
     {U16, 1},
     {FLO, 7},
     {SAT, 1},
     {NON, 0},
     {RAW, 1},
     {U16, 1},
     {DUB, 1}}  // GNSS2
}};

CsvTypeInfo csvTypeLookup(size_t group, size_t field) { return dataTypes[group][field]; }

int getMeasurementString(FaPacketExtractor& packet, const CsvTypeInfo& typeInfo, char* ptr, const uint16_t remaining)
{
    switch (typeInfo.type)
    {
        case U8:
        {
            return extractToString<uint8_t>(packet, typeInfo.len, ptr, remaining);
        }
        case U16:
        {
            return extractToString<uint16_t>(packet, typeInfo.len, ptr, remaining);
        }
        case U32:
        {
            return extractToString<uint32_t>(packet, typeInfo.len, ptr, remaining);
        }
        case U64:
        {
            return extractToString<uint64_t>(packet, typeInfo.len, ptr, remaining);
        }
        case FLO:
        {
            return extractToString<float>(packet, typeInfo.len, ptr, remaining);
        }
        case DUB:
        {
            return extractToString<double>(packet, typeInfo.len, ptr, remaining);
        }
        case UTC:
        {
            int offset = 0;
            offset += extractToString<int8_t>(packet, 1, ptr + offset, remaining - offset);
            offset += extractToString<uint8_t>(packet, 5, ptr + offset, remaining - offset);
            offset += extractToString<uint16_t>(packet, 1, ptr + offset, remaining - offset);
            return offset;
        }
        case SAT:
        {
            int offset = 0;
            offset += extractToString<uint8_t>(packet, 5, ptr + offset, remaining - offset);
            offset += snprintf(ptr + offset, remaining - offset, ",");
            offset += extractToString<int8_t>(packet, 1, ptr + offset, remaining - offset);
            offset += snprintf(ptr + offset, remaining - offset, ",");
            offset += extractToString<uint16_t>(packet, 1, ptr + offset, remaining - offset);
            return offset;
        }
        case RAW:
        {
            int offset = 0;
            offset += extractToString<uint8_t>(packet, 4, ptr + offset, remaining - offset);
            offset += snprintf(ptr + offset, remaining - offset, ",");
            offset += extractToString<int8_t>(packet, 1, ptr + offset, remaining - offset);
            offset += snprintf(ptr + offset, remaining - offset, ",");
            offset += extractToString<uint8_t>(packet, 1, ptr + offset, remaining - offset);
            offset += snprintf(ptr + offset, remaining - offset, ",");
            offset += extractToString<uint16_t>(packet, 1, ptr + offset, remaining - offset);
            offset += snprintf(ptr + offset, remaining - offset, ",");
            offset += extractToString<double>(packet, 2, ptr + offset, remaining - offset);
            offset += snprintf(ptr + offset, remaining - offset, ",");
            offset += extractToString<float>(packet, 1, ptr + offset, remaining - offset);

            return offset;
        }
        case UNK:
        {
            packet.discard(typeInfo.len);
            return 0;
        }
        default:
        {  // NON
            VN_ABORT();
        }
    };
}

const char* getMeasurementName(const size_t binaryGroup, const size_t binaryField)
{
    switch (binaryGroup)
    {
        case 0:
        {
            switch (binaryField)
            {
                case 0:
                {
                    return "timeStartup";
                }
                case 1:
                {
                    return "timeGps";
                }
                case 2:
                {
                    return "timeSyncIn";
                }
                case 3:
                {
                    return "yaw,pitch,roll";
                }
                case 4:
                {
                    return "quatX,quatY,quatZ,quatW";
                }
                case 5:
                {
                    return "angularRateX,angularRateY,angularRateZ";
                }
                case 6:
                {
                    return "lat,lon,alt";
                }
                case 7:
                {
                    return "velNedX,velNedY,velNedZ";
                }
                case 8:
                {
                    return "accelX,accelY,accelZ";
                }
                case 9:
                {
                    return "uncompAccelX,uncompAccelY,uncompAccelZ,"
                           "uncompGyroX,uncompGyroY,uncompGyroZ";
                }
                case 10:
                {
                    return "magX,magY,magZ,pressure,temperature";
                }
                case 11:
                {
                    return "deltaTime,deltaThetaX,deltaThetaY,deltaThetaZ,"
                           "deltaVelX,deltaVelY,deltaVelZ";
                }
                case 12:
                {
                    return "insStatus";
                }
                case 13:
                {
                    return "syncInCnt";
                }
                case 14:
                {
                    return "timeGpsPps";
                }
                default:
                {
                    return "";
                }
            }
        }
        case 1:
        {
            switch (binaryField)
            {
                case 0:
                {
                    return "timeStartup";
                }
                case 1:
                {
                    return "timeGps";
                }
                case 2:
                {
                    return "timeGpsTow";
                }
                case 3:
                {
                    return "timeGpsWeek";
                }
                case 4:
                {
                    return "timeSyncIn";
                }
                case 5:
                {
                    return "timeGpsPps";
                }
                case 6:
                {
                    return "utcYear,utcMonth,utcDay,utcHour,utcMinute,utcSecond,utcFracsec";
                }
                case 7:
                {
                    return "syncInCnt";
                }
                case 8:
                {
                    return "syncOutCnt";
                }
                case 9:
                {
                    return "timeStatus";
                }
                default:
                {
                    return "";
                }
            }
        }
        case 2:
        {
            switch (binaryField)
            {
                case 0:
                {
                    return "imuStatus";
                }
                case 1:
                {
                    return "uncompMagX,uncompMagY,uncompMagZ";
                }
                case 2:
                {
                    return "uncompAccelX,uncompAccelY,uncompAccelZ";
                }
                case 3:
                {
                    return "uncompGyroX,uncompGyroY,uncompGyroZ";
                }
                case 4:
                {
                    return "temperature";
                }
                case 5:
                {
                    return "pressure";
                }
                case 6:
                {
                    return "deltaTime,deltaThetaX,deltaThetaY,deltaThetaZ";
                }
                case 7:
                {
                    return "deltaVelX,deltaVelY,deltaVelZ";
                }
                case 8:
                {
                    return "magX,magY,magZ";
                }
                case 9:
                {
                    return "accelX,accelY,accelZ";
                }
                case 10:
                {
                    return "angularRateX,angularRateY,angularRateZ";
                }
                case 11:
                {
                    return "sensSat";
                }
                default:
                {
                    return "";
                }
            }
        }
        case 3:
        {
            switch (binaryField)
            {
                case 0:
                {
                    return "gps1UtcYear,gps1UtcMonth,gps1UtcDay,gps1UtcHour,gps1UtcMinute,gps1UtcSecond,gps1UtcFracsec";
                }
                case 1:
                {
                    return "gps1Tow";
                }
                case 2:
                {
                    return "gps1Week";
                }
                case 3:
                {
                    return "gnss1NumSats";
                }
                case 4:
                {
                    return "gnss1Fix";
                }
                case 5:
                {
                    return "gnss1PosLat,gnss1PosLon,gnss1PosAlt";
                }
                case 6:
                {
                    return "gnss1PosEcefX,gnss1PosEcefY,gnss1PosEcefY";
                }
                case 7:
                {
                    return "gnss1VelNedX,gnss1VelNedY,gnss1VelNedZ";
                }
                case 8:
                {
                    return "gnss1VelEcefX,gnss1VelEcefY,gnss1VelEcefZ";
                }
                case 9:
                {
                    return "gnss1PosUncertaintyN,gnss1PosUncertaintyE,gnss1PosUncertaintyD";
                }
                case 10:
                {
                    return "gnss1VelUncertainty";
                }
                case 11:
                {
                    return "gnss1TimeUncertainty";
                }
                case 12:
                {
                    return "gnss1TimeInfo";
                }
                case 13:
                {
                    return "gnss1GDop,gnss1PDop,gnss1TDop,gnss1VDop,gnss1HDop,gnss1NDop,gnss1EDop";
                }
                case 14:
                {
                    return "numSats,sys_??,svid_??,flags_??,cno_??,qi_??,el_??,az_??";
                }
                case 16:
                {
                    return "tow,week,numMeas,sys,svId,freq,chan,slot,"
                           "cno,flags,pr,cp,dp";
                }
                case 17:
                {
                    return "gnss1Status";
                }
                case 18:
                {
                    return "gnss1AltMsl";
                }
                default:
                {
                    return "";
                }
            }
        }
        case 4:
        {
            switch (binaryField)
            {
                case 0:
                {
                    return "ahrsStatus";
                }
                case 1:
                {
                    return "yaw,pitch,roll";
                }
                case 2:
                {
                    return "quatX,quatY,quatZ,quatW";
                }
                case 3:
                {
                    return "dcm00,dcm01,dcm02,dcm10,dcm11,dcm12,dcm20,dcm21,dcm22";
                }
                case 4:
                {
                    return "magNedX,magNedY,magNedZ";
                }
                case 5:
                {
                    return "accelNedX,accelNedY,accelNedZ";
                }
                case 6:
                {
                    return "linBodyAccX,linBodyAccY,linBodyAccZ";
                }
                case 7:
                {
                    return "linAccelNedX,linAccelNedY,linAccelNedZ";
                }
                case 8:
                {
                    return "yawU,pitchU,rollU";
                }
                case 12:
                {
                    return "heave,heaveRate,delayedHeave";
                }
                case 13:
                {
                    return "attU";
                }
                default:
                {
                    return "";
                }
            }
        }
        case 5:
        {
            switch (binaryField)
            {
                case 0:
                {
                    return "insStatus";
                }
                case 1:
                {
                    return "lat,lon,alt";
                }
                case 2:
                {
                    return "posEcefX,posEcefY,posEcefZ";
                }
                case 3:
                {
                    return "velBodyX,velBodyY,velBodyZ";
                }
                case 4:
                {
                    return "velNedX,velNedY,velNedZ";
                }
                case 5:
                {
                    return "velEcefX,velEcefY,velEcefZ";
                }
                case 6:
                {
                    return "magEcefX,magEcefY,magEcefZ";
                }
                case 7:
                {
                    return "accelEcefX,accelEcefY,accelEcefZ";
                }
                case 8:
                {
                    return "linAccelEcefX,linAccelEcefY,linAccelEcefZ";
                }
                case 9:
                {
                    return "posU";
                }
                case 10:
                {
                    return "velU";
                }
                default:
                {
                    return "";
                }
            }
        }
        case 6:
        {
            switch (binaryField)
            {
                case 0:
                {
                    return "gps2UtcYear,gps2UtcMonth,gps2UtcDay,gps2UtcHour,gps2UtcMinute,gps2UtcSecond,gps2UtcFracsec";
                }
                case 1:
                {
                    return "gps2Tow";
                }
                case 2:
                {
                    return "gps2Week";
                }
                case 3:
                {
                    return "gnss2NumSats";
                }
                case 4:
                {
                    return "gnss2Fix";
                }
                case 5:
                {
                    return "gnss2PosLat,gnss2PosLon,gnss2PosAlt";
                }
                case 6:
                {
                    return "gnss2PosEcefX,gnss2PosEcefY,gnss2PosEcefY";
                }
                case 7:
                {
                    return "gnss2VelNedX,gnss2VelNedY,gnss2VelNedZ";
                }
                case 8:
                {
                    return "gnss2VelEcefX,gnss2VelEcefY,gnss2VelEcefZ";
                }
                case 9:
                {
                    return "gnss2PosUncertaintyN,gnss2PosUncertaintyE,gnss2PosUncertaintyD";
                }
                case 10:
                {
                    return "gnss2VelUncertainty";
                }
                case 11:
                {
                    return "gnss2TimeUncertainty";
                }
                case 12:
                {
                    return "gnss2TimeInfo";
                }
                case 13:
                {
                    return "gnss2GDop,gnss2PDop,gnss2TDop,gnss2VDop,gnss2HDop,gnss2NDop,gnss2EDop";
                }
                case 14:
                {
                    return "numSats,sys_??,svId_??,flags_??,cno_??,qi_??,el_??,az_??";
                }
                case 16:
                {
                    return "tow,week,numMeas,sys,svId,freq,chan,slot,"
                           "cno,flags,pr,cp,dp";
                }
                case 17:
                {
                    return "gnss2Status";
                }
                case 18:
                {
                    return "gnss2AltMsl";
                }
                default:
                {
                    return "";
                }
            }
        }
        default:
        {
            VN_ABORT();
        }
    }
    VN_ABORT();
}

const char* getMeasurementString(const AsciiPacketProtocol::AsciiMeasurementHeader& msg)
{
    switch (msg)
    {
        case AsciiPacketProtocol::AsciiMeasurementHeader::YPR:
            return "yaw,pitch,roll";
        case AsciiPacketProtocol::AsciiMeasurementHeader::QTN:
            return "quatX,quatY,quatZ,quatW";
        case AsciiPacketProtocol::AsciiMeasurementHeader::QMR:
            return "quatX,quatY,quatZ,quatW,magX,magY,magZ,accelX,accelY,accelZ,angularRateX,angularRateY,angularRateX";
        case AsciiPacketProtocol::AsciiMeasurementHeader::MAG:
            return "magX,magY,magZ";
        case AsciiPacketProtocol::AsciiMeasurementHeader::ACC:
            return "accelX,accelY,accelZ";
        case AsciiPacketProtocol::AsciiMeasurementHeader::GYR:
            return "angularRateX,angularRateY,angularRateX";
        case AsciiPacketProtocol::AsciiMeasurementHeader::MAR:
            return "magX,magY,magZ,accelX,accelY,accelZ,angularRateX,angularRateY,angularRateX";
        case AsciiPacketProtocol::AsciiMeasurementHeader::YMR:
            return "yaw,pitch,roll,magX,magY,magZ,accelX,accelY,accelZ,angularRateX,angularRateY,angularRateX";
        case AsciiPacketProtocol::AsciiMeasurementHeader::YBA:
            return "yaw,pitch,roll,linBodyAccelX,linBodyAccelY,linBodyAccelZ,angularRateX,angularRateY,angularRateX";
        case AsciiPacketProtocol::AsciiMeasurementHeader::YIA:
            return "yaw,pitch,roll,linAccelNedX,linAccelNedY,linAccelNedZ,angularRateX,angularRateY,angularRateX";
        case AsciiPacketProtocol::AsciiMeasurementHeader::IMU:
            return "uncompMagX,uncompMagY,uncompMagZ,"
                   "uncompAccelX,uncompAccelY,uncompAccelZ,"
                   "uncompGyroX,uncompGyroY,uncompGyroZ,temperature,pressure";
        case AsciiPacketProtocol::AsciiMeasurementHeader::GPS:
            return "gps1Tow,gps1Week,gnss1Fix,gnss1NumSats,"
                   "gnss1PosLat,gnss1PosLon,gnss1PosAlt,"
                   "gnss1VelN,gnss1VelE,gnss1VelD,"
                   "gnss1PosUncertaintyN,gnss1PosUncertaintyE,gnss1PosUncertaintyD,"
                   "gnss1VelUncertainty,gnss1TimeUncertainty";
        case AsciiPacketProtocol::AsciiMeasurementHeader::GPE:
            return "gps1Tow,gps1Week,gnss1Fix,gnss1NumSats,"
                   "gnss1PosEcefX,gnss1PosEcefY,gnss1PosEcefZ,"
                   "gnss1VelEcefX,gnss1VelEcefY,gnss1VelEcefZ,"
                   "gnss1PosUncertaintyN,gnss1PosUncertaintyE,gnss1PosUncertaintyD,"
                   "gnss1VelUncertainty,gnss1TimeUncertainty";
        case AsciiPacketProtocol::AsciiMeasurementHeader::INS:
            return "timeGpsTow,timeGpsWeek,insStatus,yaw,pitch,roll,"
                   "posLat,posLon,posAlt,velNedN,velNedE,velNedD,attU,posU,velU";
        case AsciiPacketProtocol::AsciiMeasurementHeader::ISE:
            return "timeGpsTow,timeGpsWeek,insStatus,yaw,pitch,roll,"
                   "posEcefX,posEcefY,posEcefZ,velEcefX,velEcefY,velEcefZ,attU,posU,velU";
        case AsciiPacketProtocol::AsciiMeasurementHeader::ISL:
            return "yaw,pitch,roll,posEcefX,posEcefY,posEcefZ,velEcefX,velEcefY,velEcefZ,"
                   "accelX,accelY,accelZ,angularRateX,angularRateY,angularRateZ";
        case AsciiPacketProtocol::AsciiMeasurementHeader::DTV:
            return "deltaTime,deltaThetaX,deltaThetaY,deltaThetaZ,deltaVelX,deltaVelY,deltaVelZ";
        case AsciiPacketProtocol::AsciiMeasurementHeader::G2S:
            return "gps2Tow,gps2Week,gnss2Fix,gnss2NumSats,"
                   "gnss2PosLat,gnss2PosLon,gnss2PosAlt,"
                   "gnss2VelN,gnss2VelE,gnss2VelD,"
                   "gnss2PosUncertaintyN,gnss2PosUncertaintyE,gnss2PosUncertaintyD,"
                   "gnss2VelUncertainty,gnss2TimeUncertainty";
        case AsciiPacketProtocol::AsciiMeasurementHeader::G2E:
            return "gps2Tow,gps2Week,gnss2Fix,gnss2NumSats,"
                   "gnss2PosLat,gnss2PosLon,gnss2PosAlt,"
                   "gnss2VelN,gnss2VelE,gnss2VelD,"
                   "gnss2PosUncertaintyN,gnss2PosUncertaintyE,gnss2PosUncertaintyD,"
                   "gnss2VelUncertainty,gnss2TimeUncertainty";
        case AsciiPacketProtocol::AsciiMeasurementHeader::HVE:
            return "heave,heaveRate,delayedHeave";
        default:
            return "";
    }
}

}  // namespace VN
