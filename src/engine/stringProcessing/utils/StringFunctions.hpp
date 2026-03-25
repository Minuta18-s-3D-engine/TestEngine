#ifndef ENGINE_STRINGPROCESSING_UTILS_STRINGFUNCTIONS_HPP
#define ENGINE_STRINGPROCESSING_UTILS_STRINGFUNCTIONS_HPP

#include <string>
#include <vector>

namespace StringFunctions {
    std::string trim(const std::string& str);
    std::string trim(const std::string_view& str);

    std::vector<std::string> split(const std::string& str, char delimiter);
    std::vector<std::string> split(
        const std::string_view& str, char delimiter
    );
    
    std::string toLower(const std::string& str);
    std::string toLower(const std::string_view& str);
    
    std::string toUpper(const std::string& str);
    std::string toUpper(const std::string_view& str);

    std::string toCamelCase(const std::string& str);
    std::string toCamelCase(const std::string_view& str);

    std::string unquote(const std::string& str);
    std::string unquote(const std::string_view& str);
}

#endif // ENGINE_STRINGPROCESSING_UTILS_STRINGFUNCTIONS_HPP