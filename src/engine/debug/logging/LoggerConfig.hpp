#ifndef ENGINE_DEBUG_LOGGING_LOGGERCONFIG_HPP_
#define ENGINE_DEBUG_LOGGING_LOGGERCONFIG_HPP_

#include <vector>
#include <memory>
#include <utility>
#include <type_traits>

#include "Log.hpp"
#include "LoggerMiddleware.hpp"

struct LoggerConfig {
    LogLevel level = LogLevel::INFO;
    std::vector<std::shared_ptr<LoggerMiddleware>> middlewares;

    template <typename... Mws>
    static LoggerConfig create(LogLevel level, Mws&&... mws) {
        LoggerConfig cfg;
        cfg.level = level;
        cfg.middlewares.reserve(sizeof...(mws));

        (cfg.middlewares.push_back(
            std::make_shared<std::decay_t<Mws>>(std::forward<Mws>(mws))
        ), ...);

        return cfg;
    }
};

#endif // ENGINE_DEBUG_LOGGING_LOGGERCONFIG_HPP_
