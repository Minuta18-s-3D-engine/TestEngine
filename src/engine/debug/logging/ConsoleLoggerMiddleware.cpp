#include "ConsoleLoggerMiddleware.hpp"

void ConsoleLoggerMiddleware::log(const Log& log) {
    std::cout << log.time << "\t" 
        << logLevelMapper.toString(log.level) << "\t"
        << log.loggerName << "\t" << log.message;

    if (static_cast<int>(log.level) >= static_cast<int>(LogLevel::WARNING)) {
        std::cout << std::endl;
    } else {
        std::cout << "\n";
    }
}
