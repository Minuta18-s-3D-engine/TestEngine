#ifndef ENGINE_STRINGPROCESSING_UTILS_PARSEEXCEPTIONS_HPP
#define ENGINE_STRINGPROCESSING_UTILS_PARSEEXCEPTIONS_HPP

#include <string>
#include <vector>
#include <exception>

struct Error {
    std::string message;
    size_t line;
    size_t column;
};

using ErrorLog = std::vector<Error>;

class ErrorLogException : public std::exception {
private:
    ErrorLog errorLog;
public:
    explicit ErrorLogException(const ErrorLog& log) : errorLog(log) {}

    const char* what() const noexcept override {
        return "Parsing failed with errors";
    }

    const ErrorLog& getErrorLog() const {
        return errorLog;
    }
};

#endif // ENGINE_STRINGPROCESSING_UTILS_PARSEEXCEPTIONS_HPP
