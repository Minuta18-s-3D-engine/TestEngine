#ifndef ENGINE_UTILS_EXC_GENERALEXCEPTIONS_H_
#define ENGINE_UTILS_EXC_GENERALEXCEPTIONS_H_

#include <exception>
#include <string>

namespace exc {

class already_exists : public std::exception {
    std::string message;
public:
    already_exists(const char* _message) : message(_message) {}
    already_exists(const std::string& _message) : message(_message) {}

    const char* what() const noexcept { return message.c_str(); }
};

class file_corrupted : public std::exception {
    std::string message;
public:
    file_corrupted(const char* _message) : message(_message) {}
    file_corrupted(const std::string& _message) : message(_message) {}

    const char* what() const noexcept { return message.c_str(); }
};

class not_a_file : public std::exception {
    std::string message;
public:
    not_a_file(const char* _message) : message(_message) {}
    not_a_file(const std::string& _message) : message(_message) {}

    const char* what() const noexcept { return message.c_str(); }
};

class file_not_found : public std::exception {
    std::string message;
public:
    file_not_found(const char* _message) : message(_message) {}
    file_not_found(const std::string& _message) : message(_message) {}

    const char* what() const noexcept { return message.c_str(); }
};

class invalid_argument : public std::exception {
    std::string message;
public:
    invalid_argument(const char* _message) : message(_message) {}
    invalid_argument(const std::string& _message) : message(_message) {}

    const char* what() const noexcept { return message.c_str(); }
};

}

#endif // ENGINE_UTILS_EXC_GENERALEXCEPTIONS_H_
