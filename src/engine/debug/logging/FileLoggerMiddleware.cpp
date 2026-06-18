#include "FileLoggerMiddleware.hpp"

LoggerFilePool FileLoggerMiddleware::pool;

FileLoggerMiddleware::FileLoggerMiddleware(const VirtualPath& _logFilePath) 
  : logFilePath(_logFilePath) {
    logFile = pool.openFile(_logFilePath, File::IOMode::WRITE);
}

void FileLoggerMiddleware::log(const Log& log) {
    if (!logFile) return;

    logFile->writeFormat("{} {} {} {}\n",
        log.time,
        static_cast<int>(log.level),
        log.loggerName,
        log.message
    );

    if (static_cast<int>(log.level) >= static_cast<int>(LogLevel::WARNING)) {
        logFile->flush();
    }
}
