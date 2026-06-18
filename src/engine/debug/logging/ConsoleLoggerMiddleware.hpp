#ifndef ENGINE_DEBUG_LOGGING_CONSOLELOGGERMIDDLEWARE_HPP_
#define ENGINE_DEBUG_LOGGING_CONSOLELOGGERMIDDLEWARE_HPP_

#include <iostream>

#include "LoggerMiddleware.hpp"

class ConsoleLoggerMiddleware : public LoggerMiddleware {
public:
    explicit ConsoleLoggerMiddleware() = default;

    void log(const Log& log) override;
};

#endif // ENGINE_DEBUG_LOGGING_CONSOLELOGGERMIDDLEWARE_HPP_
