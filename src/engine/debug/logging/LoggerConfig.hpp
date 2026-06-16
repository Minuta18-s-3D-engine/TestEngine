#ifndef ENGINE_DEBUG_LOGGING_LOGGERCONFIG_HPP_
#define ENGINE_DEBUG_LOGGING_LOGGERCONFIG_HPP_

#include <vector>
#include <initializer_list>

#include "Log.hpp"
#include "LoggerMiddleware.hpp"

struct LoggerConfig {
    LogLevel level = LogLevel::INFO;
    std::vector<LoggerMiddleware> middlewares;

    LoggerConfig(std::initializer_list<LoggerMiddleware> _middlewares);
    LoggerConfig(
        std::initializer_list<LoggerMiddleware> _middlewares, LogLevel _level
    );
};

#endif // ENGINE_DEBUG_LOGGING_LOGGERCONFIG_HPP_
