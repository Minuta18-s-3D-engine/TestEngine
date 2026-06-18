#include "Logger.hpp"

Logger::Logger(
    const std::string& _loggerName,
    LoggerConfig _cfg
) : loggerName(_loggerName), cfg(std::move(_cfg)) {}

Log Logger::createLog(
    LogLevel level, 
    const std::string& userMessage
) {
    return {
        .loggerName = loggerName,
        .time = Clock::getFormattedCurrentTime(),
        .level = level,
        .message = userMessage
    };
}
