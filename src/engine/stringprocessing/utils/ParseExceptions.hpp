#ifndef ENGINE_STRINGPROCESSING_UTILS_PARSEEXCEPTIONS_HPP
#define ENGINE_STRINGPROCESSING_UTILS_PARSEEXCEPTIONS_HPP

#include <string>
#include <vector>

struct Error {
    std::string message;
    size_t line;
    size_t column;
};

using ErrorLog = std::vector<Error>;

#endif // ENGINE_STRINGPROCESSING_UTILS_PARSEEXCEPTIONS_HPP
