#ifndef ENGINE_TIME_CLOCK_HPP_
#define ENGINE_TIME_CLOCK_HPP_

#include <chrono>
#include <format>

#include "./Timestamp.hpp"

class Clock {
    inline static std::chrono::time_point<std::chrono::steady_clock> startTime;
public:
    Clock() = delete;
    ~Clock() = delete;

    static void initialize();

    static float getTimeSinceStart();
    
    static Timestamp getCurrentTime();
    static std::string getFormattedCurrentTime();
};

#endif // ENGINE_TIME_CLOCK_HPP_
