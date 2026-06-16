#ifndef ENGINE_DEBUG_LOGGING_LOGGERMIDDLEWARE_HPP_
#define ENGINE_DEBUG_LOGGING_LOGGERMIDDLEWARE_HPP_

#include "Log.hpp"

class LoggerMiddleware {
public:
    virtual ~LoggerMiddleware() = 0;

    virtual void log(const Log& log) = 0;
};

#endif // ENGINE_DEBUG_LOGGING_LOGGERMIDDLEWARE_HPP_
