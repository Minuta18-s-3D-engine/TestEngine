#ifndef ENGINE_UTILS_EXC_GENERALEXCEPTIONS_H_
#define ENGINE_UTILS_EXC_GENERALEXCEPTIONS_H_

#include <exception>
#include <string>

namespace exc {

class already_exists : std::exception {
    std::string message;
public:
    already_exists(const char* _message) : message(_message) {}
    already_exists(const std::string& _message) : message(_message) {}

    const char* what() const noexcept { return message.c_str(); }
};

}

#endif // ENGINE_UTILS_EXC_GENERALEXCEPTIONS_H_
