#ifndef ENGINE_DEBUG_LOGGING_LOGLEVEL_HPP_
#define ENGINE_DEBUG_LOGGING_LOGLEVEL_HPP_

#include <string>

#include "../../utils/EnumMapper.hpp"

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

inline const EnumMapper<LogLevel> logLevelMapper = {
    { LogLevel::DEBUG, "DEBUG" },
    { LogLevel::INFO, "INFO" },
    { LogLevel::WARNING, "WARNING" },
    { LogLevel::ERROR, "ERROR" },
    { LogLevel::FATAL, "FATAL" },
};

struct Log {
    std::string loggerName;
    std::string time;
    LogLevel level;
    std::string message;
};

#endif // ENGINE_DEBUG_LOGGING_LOGLEVEL_HPP_
