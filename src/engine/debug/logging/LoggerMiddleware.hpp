#ifndef ENGINE_DEBUG_LOGGING_LOGGERMIDDLEWARE_HPP_
#define ENGINE_DEBUG_LOGGING_LOGGERMIDDLEWARE_HPP_

#include "Log.hpp"

class LoggerMiddleware {
public:
    virtual ~LoggerMiddleware() = default;

    LoggerMiddleware() = default;
    LoggerMiddleware(LoggerMiddleware&&) = default;
    LoggerMiddleware& operator=(LoggerMiddleware&&) = default;

    LoggerMiddleware(const LoggerMiddleware&) = delete;
    LoggerMiddleware& operator=(const LoggerMiddleware&) = delete;

    virtual void log(const Log& log) = 0;
};

#endif // ENGINE_DEBUG_LOGGING_LOGGERMIDDLEWARE_HPP_
