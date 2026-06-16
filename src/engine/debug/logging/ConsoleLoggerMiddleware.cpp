#include "ConsoleLoggerMiddleware.hpp"

void ConsoleLoggerMiddleware::log(const Log& log) {
    std::cout << log.time << "\t" 
        << logLevelMapper.toString(log.level) << "\t"
        << log.loggerName << "\t" << log.message;
}
