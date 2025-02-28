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

#ifndef IMPLEMENTATION_MEASUREMENTDATATYPES_HPP
#define IMPLEMENTATION_MEASUREMENTDATATYPES_HPP

#include <cstddef>  // For size_t
#include <cstdint>
#include <array>
#include "TemplateLibrary/Vector.hpp"
#include "TemplateLibrary/String.hpp"
#include "TemplateLibrary/Matrix.hpp"
#include "Debug.hpp"

namespace VN
{

// There is one array element for each measurement group (Common is not included as it's duplicate), which each contain a bit for each measurement type
// The standard data passed around to represent which measurement types are avaialable.
using EnabledMeasurements = std::array<uint32_t, 19>;

inline bool allDataIsEnabled(const EnabledMeasurements& measurementsToCheck, const EnabledMeasurements& availableMeasurements) noexcept
{
    size_t numAvailableGroups = availableMeasurements.size();
    bool allAreEnabled = true;
    size_t idx = 0;
    for (const auto binGroup : measurementsToCheck)
    {
        allAreEnabled &=
            ((idx < numAvailableGroups) && ((binGroup & ~availableMeasurements[idx]) == 0));  // don't error if less than num so that it goes into unparsed
        ++idx;
    }
    return allAreEnabled;
}

inline bool anyDataIsEnabled(const EnabledMeasurements& measurementsToCheck, const EnabledMeasurements& availableMeasurements) noexcept
{
    size_t idx = 0;
    for (const auto binGroup : measurementsToCheck)
    {
        if ((binGroup & availableMeasurements[idx]) != 0) { return true; }
        ++idx;
    }
    return false;
}

inline EnabledMeasurements unionOf(const EnabledMeasurements& measurements1, const EnabledMeasurements& measurements2) noexcept
{
    EnabledMeasurements retVal{};
    uint8_t groupIdx = 0;
    for (auto& retValGroup : retVal)
    {
        retValGroup = measurements1[groupIdx] | measurements2[groupIdx];
        groupIdx++;
    }
    return retVal;
}

inline EnabledMeasurements intersectionOf(const EnabledMeasurements& measurements1, const EnabledMeasurements& measurements2) noexcept
{
    EnabledMeasurements retVal{};
    uint8_t groupIdx = 0;
    for (auto& retValGroup : retVal)
    {
        retValGroup = measurements1[groupIdx] & measurements2[groupIdx];
        groupIdx++;
    }
    return retVal;
}

struct InsStatus
{
    uint16_t mode : 2;
    uint16_t gnssFix : 1;
    uint16_t resv1 : 1;
    uint16_t imuErr : 1;
    uint16_t magPresErr : 1;
    uint16_t gnssErr : 1;
    uint16_t resv2 : 1;
    uint16_t gnssCompassFix : 2;
    uint16_t : 6;  // padding

    InsStatus() = default;
    InsStatus(uint16_t other) noexcept { std::memcpy(this, &other, sizeof(InsStatus)); }
    explicit operator uint16_t() const
    {
        uint16_t result;
        std::memcpy(&result, this, sizeof(InsStatus));
        return result;
    }
    InsStatus& operator=(const uint16_t& other)
    {
        std::memcpy(this, &other, sizeof(InsStatus));
        return *this;
    }
};
static_assert(sizeof(InsStatus) == 2);

struct TimeStatus
{
    struct
    {
        uint8_t towValid : 1;
        uint8_t dateValid : 1;
        uint8_t utcValid : 1;
        uint8_t : 5;  // padding
    };
    TimeStatus() = default;
    TimeStatus(const uint8_t& other) { std::memcpy(this, &other, sizeof(TimeStatus)); }
    explicit operator uint8_t() const
    {
        uint8_t result;
        std::memcpy(&result, this, sizeof(TimeStatus));
        return result;
    }
    TimeStatus& operator=(const uint8_t& other)
    {
        std::memcpy(this, &other, sizeof(TimeStatus));
        return *this;
    }
};
static_assert(sizeof(TimeStatus) == 1);

struct AhrsStatus
{
    struct
    {
        uint16_t attitudeQuality : 2;
        uint16_t gyroSaturation : 1;
        uint16_t gyroSaturationRecovery : 1;
        uint16_t magDistrubance : 2;
        uint16_t magSaturation : 1;
        uint16_t accDisturbance : 2;
        uint16_t accSaturation : 1;
        uint16_t resv1 : 1;
        uint16_t knownMagDisturbance : 1;
        uint16_t knownAccDisturbance : 1;
        uint16_t resv2 : 1;
        uint16_t : 2;  // padding
    };
    AhrsStatus() = default;
    AhrsStatus(const uint16_t& other) { std::memcpy(this, &other, sizeof(AhrsStatus)); }
    explicit operator uint16_t() const
    {
        uint16_t result;
        std::memcpy(&result, this, sizeof(AhrsStatus));
        return result;
    }
    AhrsStatus& operator=(const uint16_t& other)
    {
        std::memcpy(this, &other, sizeof(AhrsStatus));
        return *this;
    }
};
static_assert(sizeof(AhrsStatus) == 2);

struct ImuStatus
{
    struct
    {
        uint16_t gyroStatus : 2;
        uint16_t accelStatus : 2;
        uint16_t magStatus : 2;
        uint16_t presTempStatus : 2;
        uint16_t resv : 2;
        uint16_t : 6;  // padding
    };
    ImuStatus() = default;
    ImuStatus(const uint16_t& other) { std::memcpy(this, &other, sizeof(ImuStatus)); }
    explicit operator uint16_t() const
    {
        uint16_t result;
        std::memcpy(&result, this, sizeof(ImuStatus));
        return result;
    }
    ImuStatus& operator=(const uint16_t& other)
    {
        std::memcpy(this, &other, sizeof(ImuStatus));
        return *this;
    }
};
static_assert(sizeof(ImuStatus) == 2);

struct GnssStatus
{
    struct
    {
        uint16_t enabled : 1;
        uint16_t operational : 1;
        uint16_t fix : 1;
        uint16_t antennaSignalError : 1;
        uint16_t usedForNav : 1;
        uint16_t resv1 : 3;
        uint16_t dataSource : 3;
        uint16_t usedForNavCurr : 1;
        uint16_t ppsUsedForTime : 1;
        uint16_t : 3;  // padding
    };
    GnssStatus() = default;
    GnssStatus(const uint16_t& other) { std::memcpy(this, &other, sizeof(GnssStatus)); }
    explicit operator uint16_t() const
    {
        uint16_t result;
        std::memcpy(&result, this, sizeof(GnssStatus));
        return result;
    }
    GnssStatus& operator=(const uint16_t& other)
    {
        std::memcpy(this, &other, sizeof(GnssStatus));
        return *this;
    }
};
static_assert(sizeof(GnssStatus) == 2);

struct Time
{
public:
    Time() {}
    explicit Time(const uint64_t time) : _time(time) {}
    explicit Time(const double time) : _time(time * 1e9) {}
    uint64_t nanoseconds() const { return _time; }
    uint64_t microseconds() const { return _time / static_cast<uint64_t>(1e3); }
    uint64_t milliseconds() const { return _time / static_cast<uint64_t>(1e6); }
    double seconds() { return static_cast<double>(_time) / 1e9; }

private:
    uint64_t _time = 0;
};

struct Ypr
{
    Ypr() {}
    Ypr(const Vec3f& vec) : yaw(vec[0]), pitch(vec[1]), roll(vec[2]) {}
    Ypr(float yaw, float pitch, float roll) : yaw(yaw), pitch(pitch), roll(roll) {}
    float yaw = 0;
    float pitch = 0;
    float roll = 0;
};

struct DeltaTheta
{
    DeltaTheta() {}
    DeltaTheta(float deltaTime, Vec3f deltaTheta) : deltaTime(deltaTime), deltaTheta(deltaTheta) {}
    float deltaTime = 0;
    Vec3f deltaTheta = 0;
};

struct Quat
{
    Quat() {}
    Quat(Vec3f vector, float scalar) : vector(vector), scalar(scalar) {}
    Quat(Vec4f v) : vector{v(0), v(1), v(2)}, scalar(v(3)) {}
    Vec3f vector = 0;
    float scalar = 1;
};

struct Lla
{
    Lla() {}
    Lla(const Vec3d& lla) : lat(lla[0]), lon(lla[1]), alt(lla[2]) {}
    Lla(double lat, double lon, double alt) : lat(lat), lon(lon), alt(alt) {}
    double lat = 0;
    double lon = 0;
    double alt = 0;
};

struct TimeUtc
{
    int8_t year = 0;
    uint8_t month = 0;
    uint8_t day = 0;
    uint8_t hour = 0;
    uint8_t minute = 0;
    uint8_t second = 0;
    uint16_t fracSec = 0;

    TimeUtc() = default;

    TimeUtc(int8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint16_t fracsec)
        : year(year), month(month), day(day), hour(hour), minute(minute), second(second), fracSec(fracsec) {};
};

struct GnssTimeInfo
{
    uint8_t gnssTimeStatus = 0;
    int8_t leapSeconds = 0;

    GnssTimeInfo() = default;

    GnssTimeInfo(uint8_t gnssTimeStatus, int8_t leapSeconds) : gnssTimeStatus(gnssTimeStatus), leapSeconds(leapSeconds) {};
};

struct GnssDop
{
    float gDop = 0.0;
    float pDop = 0.0;
    float tDop = 0.0;
    float vDop = 0.0;
    float hDop = 0.0;
    float nDop = 0.0;
    float eDop = 0.0;

    GnssDop() = default;

    GnssDop(std::array<float, 7> arrIn) : gDop(arrIn[0]), pDop(arrIn[1]), tDop(arrIn[2]), vDop(arrIn[3]), hDop(arrIn[4]), nDop(arrIn[5]), eDop(arrIn[6]) {};
};

constexpr uint8_t GNSS_SAT_INFO_MAX_COUNT = 35;

struct GnssSatInfo
{
    uint8_t numSats = 0;
    uint8_t resv = 0;
    std::array<uint8_t, GNSS_SAT_INFO_MAX_COUNT> sys = {0};
    std::array<uint8_t, GNSS_SAT_INFO_MAX_COUNT> svId = {0};
    std::array<uint8_t, GNSS_SAT_INFO_MAX_COUNT> flags = {0};
    std::array<uint8_t, GNSS_SAT_INFO_MAX_COUNT> cno = {0};
    std::array<uint8_t, GNSS_SAT_INFO_MAX_COUNT> qi = {0};
    std::array<int8_t, GNSS_SAT_INFO_MAX_COUNT> el = {0};
    std::array<int16_t, GNSS_SAT_INFO_MAX_COUNT> az = {0};

    GnssSatInfo() = default;

    GnssSatInfo(uint8_t numSats, uint8_t resv, const std::array<uint8_t, GNSS_SAT_INFO_MAX_COUNT>& sys, const std::array<uint8_t, GNSS_SAT_INFO_MAX_COUNT>& svId,
                const std::array<uint8_t, GNSS_SAT_INFO_MAX_COUNT>& flags, const std::array<uint8_t, GNSS_SAT_INFO_MAX_COUNT>& cno,
                const std::array<uint8_t, GNSS_SAT_INFO_MAX_COUNT>& qi, const std::array<int8_t, GNSS_SAT_INFO_MAX_COUNT>& el,
                const std::array<int16_t, GNSS_SAT_INFO_MAX_COUNT>& az)
        : numSats(numSats), resv(resv), sys(sys), svId(svId), flags(flags), cno(cno), qi(qi), el(el), az(az) {};
};

constexpr uint8_t GNSS_RAW_MEAS_MAX_COUNT = 55;

struct GnssRawMeas
{
    double tow = 0.0;
    uint16_t week = 0;
    uint8_t numMeas = 0;
    uint8_t resv = 0;
    std::array<uint8_t, GNSS_RAW_MEAS_MAX_COUNT> sys = {0};
    std::array<uint8_t, GNSS_RAW_MEAS_MAX_COUNT> svId = {0};
    std::array<uint8_t, GNSS_RAW_MEAS_MAX_COUNT> band = {0};
    std::array<uint8_t, GNSS_RAW_MEAS_MAX_COUNT> chan = {0};
    std::array<int8_t, GNSS_RAW_MEAS_MAX_COUNT> freqNum = {0};
    std::array<uint8_t, GNSS_RAW_MEAS_MAX_COUNT> cno = {0};
    std::array<uint16_t, GNSS_RAW_MEAS_MAX_COUNT> flags = {0};
    std::array<double, GNSS_RAW_MEAS_MAX_COUNT> pr = {0.0};
    std::array<double, GNSS_RAW_MEAS_MAX_COUNT> cp = {0.0};
    std::array<float, GNSS_RAW_MEAS_MAX_COUNT> dp = {0.0};

    GnssRawMeas() = default;

    GnssRawMeas(double tow, uint16_t week, uint8_t numMeas, uint8_t resv, const std::array<uint8_t, GNSS_RAW_MEAS_MAX_COUNT>& sys,
                const std::array<uint8_t, GNSS_RAW_MEAS_MAX_COUNT>& svId, const std::array<uint8_t, GNSS_RAW_MEAS_MAX_COUNT>& band,
                const std::array<uint8_t, GNSS_RAW_MEAS_MAX_COUNT>& chan, const std::array<int8_t, GNSS_RAW_MEAS_MAX_COUNT>& freqNum,
                const std::array<uint8_t, GNSS_RAW_MEAS_MAX_COUNT>& cno, const std::array<uint16_t, GNSS_RAW_MEAS_MAX_COUNT>& flags,
                const std::array<double, GNSS_RAW_MEAS_MAX_COUNT>& pr, const std::array<double, GNSS_RAW_MEAS_MAX_COUNT>& cp,
                const std::array<float, GNSS_RAW_MEAS_MAX_COUNT>& dp)
        : tow(tow),
          week(week),
          numMeas(numMeas),
          resv(resv),
          sys(sys),
          svId(svId),
          band(band),
          chan(chan),
          freqNum(freqNum),
          cno(cno),
          flags(flags),
          pr(pr),
          cp(cp),
          dp(dp) {};
};

constexpr uint8_t binaryGroupMaxSize = 3;
constexpr uint8_t binaryTypeMaxSize = 10;
}  // namespace VN

#endif  // IMPLEMENTATION_MEASUREMENTDATATYPES_HPP
