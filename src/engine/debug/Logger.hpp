#ifndef ENGINE_DEBUG_LOGGER_H_
#define ENGINE_DEBUG_LOGGER_H_

#include <string>
#include <format>

#include "LogLevel.hpp"

class Logger {
    std::string loggerName;
public:
    Logger(const std::string& _loggerName);

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
    std::string completedUserMessage = std::format(
        userMessage, std::forward<Args>(args)...
    );  
}

#endif // ENGINE_DEBUG_LOGGER_H_
