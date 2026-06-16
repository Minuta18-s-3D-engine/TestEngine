#include "Logging.hpp"

LoggerConfig Logging::defaultCfg = LoggerConfig({});

void Logging::setDefaultConfig(LoggerConfig _defaultCfg) {
    defaultCfg = _defaultCfg;
}

Logger Logging::createLogger(const std::string& loggerName) {
    return Logger(loggerName, defaultCfg);
}
