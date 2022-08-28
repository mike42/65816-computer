#include "SpeedLimit.h"
#include <chrono>
#include <thread>

SpeedLimit::SpeedLimit(uint64_t cyclesPerSecond) {
    lastTime = std::chrono::high_resolution_clock::now();
    std::chrono::nanoseconds oneSecond = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<uint64_t>(1));
    cycleDuration = oneSecond / cyclesPerSecond;
}

void SpeedLimit::apply(uint64_t ticks) {
    if(ticks <= cycleCount) {
        // No CPU progress recorded, or ticks was reset
        cycleCount = ticks;
        return;
    }
    if(ticks - cycleCount < 100) {
        // Wait until some serious progress.
        return;
    }
    auto newTime = std::chrono::high_resolution_clock::now();
    // Record elapsedTime and elapsedTicks
    uint64_t elapsedTicks = ticks - cycleCount;
    std::chrono::nanoseconds elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(newTime - lastTime);
    // Determine expected elapsed time
    std::chrono::nanoseconds expectedElapsedTime = elapsedTicks * cycleDuration;
    if(expectedElapsedTime > elapsedTime) {
        // Sleep if we are running ahead of schedule
        std::this_thread::sleep_for(expectedElapsedTime - elapsedTime);
    }
    lastTime = newTime;
    cycleCount = ticks;
}
