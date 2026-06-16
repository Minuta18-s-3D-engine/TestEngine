#ifndef ENGINE_DEBUG_LOGGING_LOGGER_HPP_
#define ENGINE_DEBUG_LOGGING_LOGGER_HPP_

#include <string>
#include <format>

#include "Log.hpp"
#include "LoggerConfig.hpp"
#include "../project/FilesystemAbstraction.hpp"
#include "../../time/Clock.hpp"

class Logger {
    std::string loggerName;
    LoggerConfig cfg;

    Log createLog(
        LogLevel level, const std::string& userMessage
    );
public:
    Logger(const std::string& _loggerName, LoggerConfig _cfg);

    template <typename... Args>
    void log(LogLevel level, const std::string& userMessage, Args&&... args);
    
    template <typename... Args>
    void debug(const std::string& message, Args&&... args);
    
    template <typename... Args>
    void info(const std::string& message, Args&&... args);
    
    template <typename... Args>
    void warning(const std::string& message, Args&&... args);
    
    template <typename... Args>
    void error(const std::string& message, Args&&... args);

    template <typename... Args>
    void fatal(const std::string& message, Args&&... args);
};

Log Logger::createLog(
    LogLevel level, 
    const std::string& userMessage
) {
    return {
        .loggerName = loggerName,
        .level = level,
        .time = Clock::getFormattedCurrentTime(),
        .message = userMessage
    };
}

template <typename... Args>
void Logger::debug(const std::string& message, Args&&... args) {
    log(LogLevel::DEBUG, message, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::info(const std::string& message, Args&&... args) {
    log(LogLevel::INFO, message, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::warning(const std::string& message, Args&&... args) {
    log(LogLevel::WARNING, message, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::error(const std::string& message, Args&&... args) {
    log(LogLevel::ERROR, message, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::fatal(const std::string& message, Args&&... args) {
    log(LogLevel::FATAL, message, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::log(LogLevel level, const std::string& message, Args&&... args) {
    if (static_cast<int>(level) < static_cast<int>(cfg.level)) return;
    
    std::string completedUserMessage = std::format(
        userMessage, std::forward<Args>(args)...
    );  

    Log log = createLog(level, completedUserMessage);

    for (const auto& middleware : cfg.middlewares) {
        if (!middleware) continue; 
        
        middleware.log(log);
    }
}

#endif // ENGINE_DEBUG_LOGGING_LOGGER_HPP_
