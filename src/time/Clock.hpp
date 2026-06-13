#ifndef ENGINE_TIME_CLOCK_HPP_
#define ENGINE_TIME_CLOCK_HPP_

#include <chrono>
#include <format>

#include "./Timestamp.hpp"

class Clock {
    std::chrono::time_point<std::chrono::steady_clock> startTime;
public:
    Clock();

    float getTimeSinceStart() const;
    
    Timestamp getCurrentTime() const;
    std::string getFormattedCurrentTime() const;
};

#endif // ENGINE_TIME_CLOCK_HPP_
