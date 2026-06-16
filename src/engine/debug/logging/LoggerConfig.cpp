#include "LoggerConfig.hpp"

LoggerConfig::LoggerConfig(
    std::initializer_list<LoggerMiddleware> _middlewares
) : middlewares(_middlewares) {}

LoggerConfig::LoggerConfig(
    std::initializer_list<LoggerMiddleware> _middlewares,
    LogLevel _level
) : middlewares(_middlewares), level(_level) {}
