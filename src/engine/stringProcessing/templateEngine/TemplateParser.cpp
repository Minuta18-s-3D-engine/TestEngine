#include "TemplateParser.hpp"

void TemplateParser::processEndOfFile(
    TemplateParser::TemplateProcessingContext& context
) const {
    if (context.state != ParseState::Outside) {
        context.result.errors.push_back({
            .message = "Unclosed tag at end of file",
            .line = context.lastOpenTag.line,
            .column = context.lastOpenTag.column,
        });
    }
}

void TemplateParser::processTextToken(
    TemplateParser::TemplateProcessingContext& context 
) const {
    if (context.state == ParseState::Outside) {
        context.result.output += context.token.value;
    } else if (context.state == ParseState::InsideVariableTag) {
        std::string key = StringFunctions::trim(context.token.value);
        context.result.output += context.arguments.get(key, "");
    }
}

void TemplateParser::processVariableTagStart(
    TemplateParser::TemplateProcessingContext& context
) const {
    if (context.state != ParseState::Outside) {
        context.result.errors.push_back({
            .message = "Nested tags are not allowed",
            .line = context.token.line,
            .column = context.token.column,
        });
        if (context.stopOnError) {
            context.stopNeeded = true;
            return;
        }
    }
    context.state = ParseState::InsideVariableTag;
    context.lastOpenTag = context.token;
}

void TemplateParser::processVariableTagEnd(
    TemplateParser::TemplateProcessingContext& context
) const {
    if (context.state != ParseState::InsideVariableTag) {
        context.result.errors.push_back({
            .message = "Unexpected closing variable tag",
            .line = context.token.line,
            .column = context.token.column,
        });
        if (context.stopOnError) {
            context.stopNeeded = true;
            return;
        }
    }
    context.state = ParseState::Outside;
}

void TemplateParser::processCommentTagStart(
    TemplateParser::TemplateProcessingContext& context
) const {
    if (context.state != ParseState::Outside) {
        context.result.errors.push_back({
            .message = "Nested tags are not allowed",
            .line = context.token.line,
            .column = context.token.column,
        });
        if (context.stopOnError) {
            context.stopNeeded = true;
            return;
        }
    }
    context.state = ParseState::InsideCommentTag;
    context.lastOpenTag = context.token;
}

void TemplateParser::processCommentTagEnd(
    TemplateParser::TemplateProcessingContext& context
) const {
    if (context.state != ParseState::InsideCommentTag) {
        context.result.errors.push_back({
            .message = "Unexpected closing comment tag",
            .line = context.token.line,
            .column = context.token.column,
        });
        if (context.stopOnError) {
            context.stopNeeded = true;
            return;
        }
    }
    context.state = ParseState::Outside;
}

TemplateParser::ProcessResult TemplateParser::processTemplate(
    const std::string& fileContents,
    const ParserArguments& arguments,
    bool stopOnError
) const {
    TemplateProcessingContext context(fileContents, arguments, stopOnError);

    while (true) {
        TemplateToken token = context.lexer.nextToken();
        context.token = token;
        if (token.type == TemplateTokenType::EndOfFile) {
            processEndOfFile(context);
            break;
        }

        switch (token.type) {
            case TemplateTokenType::Text:
                processTextToken(context); break;
            case TemplateTokenType::VariableTagStart:
                processVariableTagStart(context); break;
            case TemplateTokenType::VariableTagEnd:
                processVariableTagEnd(context); break;
            case TemplateTokenType::CommentTagStart:
                processCommentTagStart(context); break;
            case TemplateTokenType::CommentTagEnd:
                processCommentTagEnd(context); break;
            default:
                context.result.errors.push_back({
                    .message = "Unexpected token type",
                    .line = context.token.line,
                    .column = context.token.column,
                });
                if (context.stopOnError) {
                    context.stopNeeded = true;
                }
        }

        if (context.stopNeeded) {
            return context.result;
        }
    }

    return context.result;
}

ErrorLog TemplateParser::validateFromString(
    const std::string& fileContents
) const {
    TemplateArguments emptyArgs;
    return processTemplate(fileContents, emptyArgs, false).errors;
}

std::string TemplateParser::renderFromString(
    const std::string& fileContents,
    const ParserArguments& arguments
) const {
    ProcessResult result = processTemplate(fileContents, arguments, true);
    if (!result.errors.empty()) {
        throw ErrorLogException(result.errors);
    }
    return result.output;
}


