#ifndef ENGINE_STRINGPROCESSING_TEMPLATEENGINE_TEMPLATEPARSER_HPP
#define ENGINE_STRINGPROCESSING_TEMPLATEENGINE_TEMPLATEPARSER_HPP

#include "../utils/Parser.hpp"
#include "TemplateLexer.hpp"

class TemplateParser : public Parser {
private:
    struct ProcessResult {
        std::string output;
        ErrorLog errors;
    };

    enum class ParserState {
        Outside,
        InsideVariableTag,
        InsideCommentTag,
    };

    ProcessResult processTemplate(
        const std::string& fileContents,
        const ParserArguments& arguments,
        bool stepOnError
    ) const;
public:
    ErrorLog validateFromString(
        const std::string& fileContents
    ) const override;

    std::string renderFromString(
        const std::string& fileContents,
        const ParserArguments& arguments
    ) const override;
};

#endif // ENGINE_STRINGPROCESSING_TEMPLATEENGINE_TEMPLATEPARSER_HPP