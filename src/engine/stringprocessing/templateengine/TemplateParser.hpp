#ifndef ENGINE_STRINGPROCESSING_TEMPLATEENGINE_TEMPLATEPARSER_HPP
#define ENGINE_STRINGPROCESSING_TEMPLATEENGINE_TEMPLATEPARSER_HPP

#include "../utils/Parser.hpp"

class TemplateParser : public Parser {
private:
    const std::string startingVariableTag = "{{";
    const std::string endingVariableTag = "}}";
public:
    ErrorLog validateFromString(
        const std::string& fileContents
    ) const override;

    void renderFromString(
        const std::string& fileContents,
        const ParserArguments& arguments
    ) const override;
};

#endif // ENGINE_STRINGPROCESSING_TEMPLATEENGINE_TEMPLATEPARSER_HPP