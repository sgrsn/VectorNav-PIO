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

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <stdint.h>
#include <array>
#include "Implementation/BinaryMeasurementDefinitions.hpp"
#include "Implementation/MeasurementDatatypes.hpp"
#include "HAL/Duration.hpp"

namespace VN
{

#ifndef TIME_GROUP_ENABLE
#define TIME_GROUP_ENABLE                                                                                                                                  \
    (TIME_TIMESTARTUP_BIT | TIME_TIMEGPS_BIT | TIME_TIMEGPSTOW_BIT | TIME_TIMEGPSWEEK_BIT | TIME_TIMESYNCIN_BIT | TIME_TIMEGPSPPS_BIT | TIME_TIMEUTC_BIT | \
     TIME_SYNCINCNT_BIT | TIME_SYNCOUTCNT_BIT | TIME_TIMESTATUS_BIT)
#endif
#ifndef IMU_GROUP_ENABLE
#define IMU_GROUP_ENABLE                                                                                                                              \
    (IMU_IMUSTATUS_BIT | IMU_UNCOMPMAG_BIT | IMU_UNCOMPACCEL_BIT | IMU_UNCOMPGYRO_BIT | IMU_TEMPERATURE_BIT | IMU_PRESSURE_BIT | IMU_DELTATHETA_BIT | \
     IMU_DELTAVEL_BIT | IMU_MAG_BIT | IMU_ACCEL_BIT | IMU_ANGULARRATE_BIT | IMU_SENSSAT_BIT)
#endif
#ifndef GNSS_GROUP_ENABLE
#define GNSS_GROUP_ENABLE                                                                                                                                      \
    (GNSS_GNSS1TIMEUTC_BIT | GNSS_GPS1TOW_BIT | GNSS_GPS1WEEK_BIT | GNSS_GNSS1NUMSATS_BIT | GNSS_GNSS1FIX_BIT | GNSS_GNSS1POSLLA_BIT | GNSS_GNSS1POSECEF_BIT | \
     GNSS_GNSS1VELNED_BIT | GNSS_GNSS1VELECEF_BIT | GNSS_GNSS1POSUNCERTAINTY_BIT | GNSS_GNSS1VELUNCERTAINTY_BIT | GNSS_GNSS1TIMEUNCERTAINTY_BIT |              \
     GNSS_GNSS1TIMEINFO_BIT | GNSS_GNSS1DOP_BIT | GNSS_GNSS1STATUS_BIT | GNSS_GNSS1ALTMSL_BIT)
#endif
#ifndef ATTITUDE_GROUP_ENABLE
#define ATTITUDE_GROUP_ENABLE                                                                                                                \
    (ATTITUDE_YPR_BIT | ATTITUDE_QUATERNION_BIT | ATTITUDE_DCM_BIT | ATTITUDE_MAGNED_BIT | ATTITUDE_ACCELNED_BIT | ATTITUDE_LINBODYACC_BIT | \
     ATTITUDE_LINACCELNED_BIT | ATTITUDE_YPRU_BIT | ATTITUDE_HEAVE_BIT | ATTITUDE_ATTU_BIT)
#endif
#ifndef INS_GROUP_ENABLE
#define INS_GROUP_ENABLE                                                                                                                               \
    (INS_INSSTATUS_BIT | INS_POSLLA_BIT | INS_POSECEF_BIT | INS_VELBODY_BIT | INS_VELNED_BIT | INS_VELECEF_BIT | INS_MAGECEF_BIT | INS_ACCELECEF_BIT | \
     INS_LINACCELECEF_BIT | INS_POSU_BIT | INS_VELU_BIT)
#endif
#ifndef GNSS2_GROUP_ENABLE
#define GNSS2_GROUP_ENABLE                                                                                                                     \
    (GNSS2_GNSS2TIMEUTC_BIT | GNSS2_GPS2TOW_BIT | GNSS2_GPS2WEEK_BIT | GNSS2_GNSS2NUMSATS_BIT | GNSS2_GNSS2FIX_BIT | GNSS2_GNSS2POSLLA_BIT |   \
     GNSS2_GNSS2POSECEF_BIT | GNSS2_GNSS2VELNED_BIT | GNSS2_GNSS2VELECEF_BIT | GNSS2_GNSS2POSUNCERTAINTY_BIT | GNSS2_GNSS2VELUNCERTAINTY_BIT | \
     GNSS2_GNSS2TIMEUNCERTAINTY_BIT | GNSS2_GNSS2TIMEINFO_BIT | GNSS2_GNSS2DOP_BIT | GNSS2_GNSS2STATUS_BIT | GNSS2_GNSS2ALTMSL_BIT)
#endif
#ifndef GNSS3_GROUP_ENABLE
#define GNSS3_GROUP_ENABLE 0
#endif

#ifndef THREADING_ENABLE
#define THREADING_ENABLE true
#endif

namespace Config
{

namespace PacketFinders
{
// Universal
constexpr uint64_t mainBufferCapacity = 2048;
constexpr uint8_t maxNumPacketFinders = 3;                      // FA , Ascii and FB
constexpr size_t skippedReceivedByteBufferMaxPutLength = 1024;  // bytes in a single loop

// Fa
constexpr uint16_t faPacketMaxLength = 2000;
constexpr uint8_t gnssSatInfoMaxCount = GNSS_SAT_INFO_MAX_COUNT;  // Defiend in MeasurementDatatypes.hpp to avoid circular dependancy
constexpr uint8_t gnssRawMeasMaxCount = GNSS_RAW_MEAS_MAX_COUNT;  // Defiend in MeasurementDatatypes.hpp to avoid circular dependancy

// Ascii
constexpr uint8_t asciiMaxFieldCount = 34;
constexpr uint16_t asciiHeaderMaxLength = 10;
constexpr uint16_t asciiPacketMaxLength = 256;
constexpr uint8_t asciiFieldMaxLength = 20;

// Fb
constexpr size_t fbPacketMaxLength = 500;
constexpr uint64_t fbBufferCapacity = fbPacketMaxLength * 2;
}  // namespace PacketFinders

namespace PacketDispatchers
{
// Universal
constexpr EnabledMeasurements cdEnabledMeasTypes = {
    TIME_GROUP_ENABLE, IMU_GROUP_ENABLE, GNSS_GROUP_ENABLE, ATTITUDE_GROUP_ENABLE, INS_GROUP_ENABLE, GNSS2_GROUP_ENABLE, 0, 0, 0, 0, 0, GNSS3_GROUP_ENABLE};
constexpr uint8_t compositeDataQueueCapacity = 20;

// Fa
constexpr uint8_t faPacketSubscriberCapacity = 5;

// Ascii
constexpr uint8_t asciiPacketSubscriberCapacity = 5;
}  // namespace PacketDispatchers

namespace Serial
{
constexpr uint64_t numBytesToReadPerGetData = 2000;
constexpr size_t PortNameMaxLength = 32;
}  // namespace Serial

namespace Sensor
{
// Timers
constexpr Microseconds commandSendTimeoutLength = 100ms;
constexpr Microseconds wnvSendTimeoutLength = 1200ms;
constexpr Microseconds getMeasurementTimeoutLength = 100ms;

// Sleeps
constexpr Microseconds resetSleepDuration = 2500ms;
constexpr Microseconds listenSleepDuration = 1ms;
constexpr Microseconds getMeasurementSleepDuration = 100us;
constexpr Microseconds commandSendSleepDuration = 100us;

// Retries
constexpr uint8_t commandSendRetriesAllowed = 2;
constexpr bool retryVerifyConnectivity = true;
}  // namespace Sensor

namespace CommandProcessor
{
constexpr Microseconds commandRemovalTimeoutLength = Sensor::commandSendTimeoutLength * 2;
constexpr uint8_t commandProcQueueCapacity = 10;
constexpr size_t messageMaxLength = 280;
}  // namespace CommandProcessor

namespace Errors
{
constexpr uint8_t asyncErrorQueueCapacity = 5;
constexpr uint16_t asyncErrorMessageCapacity = 256;
}  // namespace Errors

// Common sense checks
static_assert(CommandProcessor::messageMaxLength >= PacketFinders::asciiPacketMaxLength);  // No use to find a packet larger than we can propagate
static_assert(PacketFinders::asciiPacketMaxLength > PacketFinders::asciiFieldMaxLength);
static_assert(PacketFinders::asciiPacketMaxLength > PacketFinders::asciiHeaderMaxLength);
static_assert(PacketFinders::mainBufferCapacity >= Serial::numBytesToReadPerGetData);

}  // namespace Config

}  // namespace VN

#endif  // CONFIG_HPP
