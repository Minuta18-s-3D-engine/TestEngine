#ifndef ENGINE_STRINGPROCESSING_TEMPLATEENGINE_TEMPLATEPARSER_HPP
#define ENGINE_STRINGPROCESSING_TEMPLATEENGINE_TEMPLATEPARSER_HPP

#include "../utils/Parser.hpp"
#include "TemplateLexer.hpp"
#include "TemplateArguments.hpp"
#include "../utils/StringFunctions.hpp"

class TemplateParser : public Parser {
private:
    struct ProcessResult {
        std::string output;
        ErrorLog errors;
    };

    enum class ParseState {
        Outside,
        InsideVariableTag,
        InsideCommentTag,
    };

    struct TemplateProcessingContext {
        ProcessResult result;
        TemplateLexer lexer;
        ParseState state;
        TemplateToken lastOpenTag;
        TemplateToken token;
        const ParserArguments& arguments;
        bool stopNeeded = false;
        bool stopOnError = false;

        TemplateProcessingContext(
            const std::string& fileContents, const ParserArguments& args,
            bool stopOnError = false
        ) : lexer(fileContents), state(ParseState::Outside), arguments(args), 
            stopOnError(stopOnError) {}
    };

    void processEndOfFile(TemplateProcessingContext& context) const;
    void processTextToken(TemplateProcessingContext& context) const;
    void processVariableTagStart(TemplateProcessingContext& context) const;
    void processVariableTagEnd(TemplateProcessingContext& context) const;
    void processCommentTagStart(TemplateProcessingContext& context) const;
    void processCommentTagEnd(TemplateProcessingContext& context) const;

    ProcessResult processTemplate(
        const std::string& fileContents,
        const ParserArguments& arguments,
        bool stopOnError
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