#ifndef ENGINE_UTILS_EXC_IMPORTEREXCEPTIONS_H_
#define ENGINE_UTILS_EXC_IMPORTEREXCEPTIONS_H_

#include <exception>
#include <string>

namespace exc {

namespace importer_exceptions {

class importer_exception : public std::exception {
protected:
    std::string message;
public:
    explicit importer_exception(const char* _message) : message(_message) {}
    explicit importer_exception(std::string _message) : message(std::move(_message)) {}

    [[nodiscard]] const char* what() const noexcept override {
        return message.c_str(); 
    } 
};

class meta_file_not_found : public importer_exception {
public:
    explicit meta_file_not_found(const char* _message)
        : importer_exception(_message) {}
    explicit meta_file_not_found(const std::string& _message)
        : importer_exception(_message) {}
};

class meta_file_invalid : public importer_exception {
public:
    explicit meta_file_invalid(const char* _message)
        : importer_exception(_message) {}
    explicit meta_file_invalid(const std::string& _message)
        : importer_exception(_message) {}
};

class resource_not_found : public importer_exception {
public:
    explicit resource_not_found(const char* _message)
        : importer_exception(_message) {}
    explicit resource_not_found(const std::string& _message)
        : importer_exception(_message) {}
};

class resource_invalid : public importer_exception {
public:
    explicit resource_invalid(const char* _message)
        : importer_exception(_message) {}
    explicit resource_invalid(const std::string& _message)
        : importer_exception(_message) {}
};

};

};

#endif // ENGINE_UTILS_EXC_IMPORTEREXCEPTIONS_H_
