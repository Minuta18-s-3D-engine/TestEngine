#include "StringFunctions.hpp"

std::string StringFunctions::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

std::string StringFunctions::trim(const std::string_view& str) {
    return trim(std::string(str));
}

std::vector<std::string> StringFunctions::split(
    const std::string& str, char delimiter
) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = str.find(delimiter);
    while (end != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }
    result.push_back(str.substr(start));
    return result;
}

std::vector<std::string> StringFunctions::split(
    const std::string_view& str, char delimiter
) {
    return split(std::string(str), delimiter);
}

std::string StringFunctions::toLower(const std::string& str) {
    std::string result = str;
    for (char& c : result) {
        c = std::tolower(c);
    }
    return result;
}

std::string StringFunctions::toLower(const std::string_view& str) {
    return toLower(std::string(str));
}

std::string StringFunctions::toUpper(const std::string& str) {
    std::string result = str;
    for (char& c : result) {
        c = std::toupper(c);
    }
    return result;
}

std::string StringFunctions::toUpper(const std::string_view& str) {
    return toUpper(std::string(str));
}

std::string StringFunctions::toCamelCase(const std::string& str) {
    std::string result;
    bool capitalizeNext = false;
    for (char c : str) {
        if (std::isspace(c) || c == '_' || c == '-') {
            capitalizeNext = true;
            continue;
        }
         
        if (capitalizeNext) {
            result += std::toupper(c);
            capitalizeNext = false;
        } else {
            result += std::tolower(c);
        }
    }
    return result;
}

std::string StringFunctions::toCamelCase(const std::string_view& str) {
    return toCamelCase(std::string(str));
}

std::string StringFunctions::unquote(const std::string& str) {
    if (str.size() < 2) return str;
    if (str[0] == '"' && str[str.size() - 1] == '"') 
        return str.substr(1, str.size() - 2);
    return str;
}

std::string StringFunctions::unquote(const std::string_view& str) {
    return unquote(std::string(str));
} 

std::string StringFunctions::sanitizeFilename(const std::string& filename) {
    std::string prohibitedChars = "\\/:*%.,;= ?\"<>|";
    std::string sanitized = "";
    for (const char& c : filename) {
        if (prohibitedChars.find(c) != std::string::npos) {
            sanitized += '_';
        } else {
            sanitized += c;
        }
    }
    return sanitized;
}
