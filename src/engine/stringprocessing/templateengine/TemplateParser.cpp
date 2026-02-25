#include "TemplateParser.hpp"

TemplateParser::ProcessResult TemplateParser::processTemplate(
    const std::string& fileContents,
    const ParserArguments& arguments,
    bool stepOnError
) const {
    ProcessResult result;
    result.output.reserve(fileContents.size());
    TemplateLexer lexer(fileContents);
    ParserState state = ParserState::Outside;
    TemplateToken lastOpenTag;

    while (true) {
        TemplateToken token = lexer.nextToken();
        if (token.type == TemplateTokenType::EndOfFile) {
            if (state != ParserState::Outside) {
                result.errors.push_back({
                    .message = getTagNameByType(lastOpenTag.type) + 
                        " tag was never closed",
                    .line = lastOpenTag.line,
                    .column = lastOpenTag.column,
                });
            }
            break;
        }

        switch (token.type) {
            case TemplateTokenType::Text:
                if (state == ParserState::Outside) {
                    result.output += token.value;
                } else if (state == ParserState::InsideVariableTag) {
                    // std::string key = token.value;
                }
                break;

        }
    }
}

ErrorLog TemplateParser::validateFromString(
    const std::string& fileContents
) const {

}
