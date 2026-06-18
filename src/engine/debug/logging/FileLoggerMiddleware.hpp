#ifndef DEFINE_DEBUG_LOGGING_FILELOGGERMIDDLEWARE_HPP_
#define DEFINE_DEBUG_LOGGING_FILELOGGERMIDDLEWARE_HPP_

#include "LoggerMiddleware.hpp"
#include "LoggerFilePool.hpp"
#include "../../project/VirtualPath.hpp"

class FileLoggerMiddleware : public LoggerMiddleware {
    VirtualPath logFilePath;
    std::shared_ptr<File> logFile;

    static LoggerFilePool pool;
public:
    explicit FileLoggerMiddleware(const VirtualPath& _logFilePath);

    void log(const Log& log) override;

    VirtualPath getLogFilePath() const { return logFilePath; }
};

#endif // DEFINE_DEBUG_LOGGING_FILELOGGERMIDDLEWARE_HPP_
