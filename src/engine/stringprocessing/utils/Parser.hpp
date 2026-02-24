#ifndef ENGINE_STRINGPROCESSING_UTILS_PARSER_HPP
#define ENGINE_STRINGPROCESSING_UTILS_PARSER_HPP

#include "ParseExceptions.hpp"

class ParserArguments {
public:
    virtual ParserArguments& set(
        const std::string& key, const std::string& value
    ) = 0;
    virtual std::string get(
        const std::string& key, const std::string& defaultValue
    ) const = 0;
};

class Parser {
public:
    virtual ErrorLog validateFromString(
        const std::string& fileContents
    ) const = 0;

    virtual std::string renderFromString(
        const std::string& fileContents,
        const ParserArguments& arguments
    ) const = 0;
};

#endif // ENGINE_STRINGPROCESSING_UTILS_PARSER_HPP
