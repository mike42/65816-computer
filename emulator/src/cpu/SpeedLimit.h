#ifndef EMULATOR_SPEEDLIMIT_H
#define EMULATOR_SPEEDLIMIT_H


#include <cstdint>
#include <chrono>

/**
 * Rate-limit CPU execution to match speed of actual hardware.
 */
class SpeedLimit {

public:
    explicit SpeedLimit(uint64_t cyclesPerSecond);

    void apply(uint64_t cyclesExecuted);

private:

    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> lastTime;
    uint64_t cycleCount;
    std::chrono::nanoseconds cycleDuration;
};


#endif //EMULATOR_SPEEDLIMIT_H
