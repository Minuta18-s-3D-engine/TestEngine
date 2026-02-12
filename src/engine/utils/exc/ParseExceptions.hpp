#ifndef ENGINE_UTILS_EXC_PARSEEXCEPTIONS_H_
#define ENGINE_UTILS_EXC_PARSEEXCEPTIONS_H_

#include <exception>
#include <string>

namespace exc {

class file_not_found : std::exception {
    std::string message;
public:
    file_not_found(const char* _message) : message(_message) {}
    file_not_found(const std::string& _message) : message(_message) {}

    const char* what() const noexcept { return message.c_str(); }
};

class parse_error : std::exception {
    std::string message;
public:
    parse_error(const char* _message) : message(_message) {}
    parse_error(const std::string& _message) : message(_message) {}

    const char* what() const noexcept { return message.c_str(); }
};

class validation_error : std::exception {
    std::string message;
public:
    validation_error(const char* _message) : message(_message) {}
    validation_error(const std::string& _message) : message(_message) {}

    const char* what() const noexcept { return message.c_str(); }
};

}

#endif // ENGINE_UTILS_EXC_PARSEEXCEPTIONS_H_
