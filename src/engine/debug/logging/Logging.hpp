#ifndef ENGINE_DEBUG_LOGGING_LOGGING_HPP_
#define ENGINE_DEBUG_LOGGING_LOGGING_HPP_

#include "Logger.hpp"
#include "LoggerConfig.hpp"

class Logging {
    static LoggerConfig defaultCfg;
public:
    static void setDefaultConfig(LoggerConfig _defaultCfg);

    static Logger createLogger(const std::string& loggerName);
};

#endif // ENGINE_DEBUG_LOGGING_LOGGING_HPP_
