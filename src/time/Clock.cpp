#include "Clock.hpp"

Clock::Clock() {
    startTime = std::chrono::steady_clock::now();
}

float Clock::getTimeSinceStart() const {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = now - startTime;
    return elapsed.count();
}

Timestamp Clock::getCurrentTime() const {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
        now.time_since_epoch()
    ).count();
}

std::string Clock::getFormattedCurrentTime() const {
    auto now = std::chrono::system_clock::now();
    return std::format(
        "{:%T}", std::chrono::floor<std::chrono::milliseconds>(now)
    );
}

