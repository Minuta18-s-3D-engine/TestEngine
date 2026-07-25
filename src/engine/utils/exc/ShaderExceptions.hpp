#ifndef TESTENG_SHADEREXCEPTIONS_HPP
#define TESTENG_SHADEREXCEPTIONS_HPP

#include <exception>
#include <string>

namespace exc {

namespace shader_exceptions {

class shader_exception : public std::exception {
protected:
    std::string message;
public:
    explicit shader_exception(const char* _message) : message(_message) {}
    explicit shader_exception(std::string _message) : message(std::move(_message)) {}

    [[nodiscard]] const char* what() const noexcept override {
        return message.c_str();
    }
};

class compilation_failed : public shader_exception {
public:
    explicit compilation_failed(const char* _message)
        : shader_exception(_message) {}
    explicit compilation_failed(std::string _message)
        : shader_exception(std::move(_message)) {}
};

class linking_failed : public shader_exception {
public:
    explicit linking_failed(const char* _message)
        : shader_exception(_message) {}
    explicit linking_failed(std::string _message)
        : shader_exception(std::move(_message)) {}
};

};

};

#endif //TESTENG_SHADEREXCEPTIONS_HPP
