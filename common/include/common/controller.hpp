#pragma once

#include <cstdint>

#include <cmath>

/** Number of times per second the controller updates.
 *
 *  Fields that reference a speed/velocity/rate (e.g. the XS command) express this speed as a 32-bit number representing
 *  the number of ticks the motor encoder should advance per loop, multiplied by 2^16 (= 65,536) to express fractional
 *  values.For example, if you want to advance the motor 1,000 counts per second, this corresponds to:
 * <pre>
 *  1,000 / 1953 ~= 0.512 counts per loop
 *   which is expressed as the integer value:
 *  round(0.512 * 65,536) = 33,555
 * </pre>
 */
constexpr const int32_t kControllerLoopsPerSecond = 1953;

/** a "grey" will be the name of the unit of motor angle the controller understands in the protocol.  It is ment to allow
 * fractional motor cycles to be represented in integer value.
 */
constexpr const int32_t kGreysPerMotorCount = 65'536;

constexpr double kGreysPerControllerLoopsPerMotorCountsPerSecond = static_cast<double>(kGreysPerMotorCount) / static_cast<double>(kControllerLoopsPerSecond);

constexpr double kControlLoopsPerSecondPerGreyPerMotorCount = static_cast<double>(kControllerLoopsPerSecond) / static_cast<double>(kGreysPerMotorCount);

/**
 * Ticks and motor counts appear to be the same unit.
 */
constexpr int32_t DegreesPerSecond2Greys(int32_t degrees_per_second, int32_t ticks_per_revolution) {
  return static_cast<int32_t>(std::round(degrees_per_second / 360.0 /* deg/revolution */ * ticks_per_revolution * kGreysPerControllerLoopsPerMotorCountsPerSecond));
}


