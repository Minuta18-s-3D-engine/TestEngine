#include "Logger.hpp"

Logger::Logger(
    const std::string& _loggerName,
    LoggerConfig _cfg
) : loggerName(_loggerName), cfg(_cfg) {}
