#include "PreprocessorParser.hpp"

PreprocessorParser::PreprocessorParser(
    const std::string& _source
) : source(_source) {}

void PreprocessorParser::makeException(
    const PreprocessorLexer::Token& token, 
    const std::string& message
) const {
    std::string what = "Preprocessor error at line " + 
        std::to_string(token.line) + ", col " + std::to_string(token.column) + 
        ": " + message + ".";
    throw exc::parse_error(what);
};

void PreprocessorParser::exceptToken(
    const PreprocessorLexer::Token& token,
    PreprocessorLexer::TokenType expectedType,
    const std::string& what
) const {
    if (token.type != expectedType) {
        makeException(token, "Expected " + what);
    }
}

void PreprocessorParser::notExceptToken(
    const PreprocessorLexer::Token& token,
    PreprocessorLexer::TokenType notExpectedType,
    const std::string& what
) const {
    if (token.type == notExpectedType) {
        makeException(token, "Not expected " + what);
    }
}

std::string PreprocessorParser::constructDirectiveName(
    const Directive& d
) const {
    if (d.name.size() < 1) return "";
    std::string directiveName = std::string(d.name[0]);
    for (int i = 1; i < d.name.size(); ++i) {
        directiveName += "." + std::string(d.name[i]);
    }

    return directiveName;
}

PreprocessorParser::DirectiveArg PreprocessorParser::requireArg(
    const Directive& d, 
    size_t index, 
    ArgType expectedType,
    size_t exceptedArgumentsCount
) const {
    if (index >= d.args.size()) {
        std::string what = "Directive '" + constructDirectiveName(d) + 
            "' expects " + std::to_string(exceptedArgumentsCount) + 
            " found " + std::to_string(d.args.size());
        makeException(d.tokens[0], what);
    }

    const auto& arg = d.args[index];
    if (arg.type != expectedType) {
        std::string what = "Argument " + std::to_string(index + 1) + 
            ": Excepted type " + 
            argTypeMapper.toString(expectedType) + ", found type " +
            argTypeMapper.toString(arg.type);
        makeException(d.tokens[0], what);
    }
    return arg;
}

PreprocessorParser::Directive PreprocessorParser::parseDirective(
    PreprocessorLexer& lexer,
    const PreprocessorLexer::Token& directiveToken
) {
    Directive result;
    result.position = {
        .line = directiveToken.line,
        .col = directiveToken.column,
        .pos = directiveToken.position
    };

    result.tokens.push_back(directiveToken);
    auto token = lexer.nextToken();
    while (
        token.type != PreprocessorLexer::TokenType::Code &&
        token.type != PreprocessorLexer::TokenType::EndOfFile
    ) {
        result.tokens.push_back(token);
        token = lexer.nextToken();
    }

    exceptToken(
        result.tokens[0], 
        PreprocessorLexer::TokenType::Identifier, 
        "directive name"
    );

    bool prevDot = false;
    size_t bracketIndex = -1;
    for (size_t i = 1; i < result.tokens.size(); ++i) {
        if (result.tokens[i].type == PreprocessorLexer::TokenType::LBracket) {
            bracketIndex = i;
            if (prevDot) {
                makeException(result.tokens[i - 1], "Invalid syntax");
            }
            break;
        }
        if (prevDot) {
            exceptToken(
                result.tokens[i], PreprocessorLexer::TokenType::Dot,
                "dot separator"
            );
            prevDot = true;
        } else {
            exceptToken(
                result.tokens[i], PreprocessorLexer::TokenType::Identifier,
                "name"
            );
            result.name.push_back(std::string_view(result.tokens[i].value));
            prevDot = false;
        }
    }

    if (bracketIndex == -1) {
        return result;
    }

    bool exceptArg = true;
    for (size_t i = bracketIndex + 1; i < result.tokens.size(); ++i) {
        const auto& t = result.tokens[i];

        if (t.type == PreprocessorLexer::TokenType::RBracket) {
            break;
        }

        if (exceptArg) {
            DirectiveArg arg;
            arg.position = { 
                .line = t.line, 
                .col = t.column, 
                .pos = t.position
            };
            arg.value = t.value;

            if (t.type == PreprocessorLexer::TokenType::Number) {
                arg.type = ArgType::Number;
            } else if (t.type == PreprocessorLexer::TokenType::String) {
                arg.type = ArgType::String;
            } else if (t.type == PreprocessorLexer::TokenType::Identifier) {
                if (t.value == "true" || t.value == "false") { 
                    arg.type = ArgType::Bool;
                } else { 
                    arg.type = ArgType::Identifier;
                }
            } else {
                makeException(t, "Invalid argument type");
            } 

            result.args.push_back(arg);
            exceptArg = false;
        } else {
            if (t.type == PreprocessorLexer::TokenType::Comma) {
                exceptArg = true;
            } else {
                makeException(t, "Expected ',' or ')'");
            }
        }
    }

    if (result.tokens.back().type != PreprocessorLexer::TokenType::RBracket) {
        makeException(result.tokens.back(), "Missing closing ')'");
    }
    return result;
}

PreprocessorParser::ParseResult PreprocessorParser::parse() {
    PreprocessorParser::ParseResult result;
    result.source = this->source;
    PreprocessorLexer lexer(result.source);

    const std::string noSectionTag = "none";
    SectionBlock currentSection;
    currentSection.type = noSectionTag;
    auto token = lexer.nextToken();

    while (token.type != PreprocessorLexer::TokenType::EndOfFile) {
        if (token.type == PreprocessorLexer::TokenType::Code) {
            result.code += std::string(token.value);
        } else if (
            token.type == PreprocessorLexer::TokenType::DirectiveMarker
        ) {
            Directive dir = parseDirective(lexer, token);

            if (dir.nameMatch({ "section" })) {
                if (!currentSection.directives.empty()) {
                    result.sections.push_back(std::move(currentSection));
                }

                currentSection = SectionBlock();
                currentSection.type = requireArg(
                    dir, 0, PreprocessorParser::ArgType::String, 1
                ).value;
                currentSection.type = 
                    StringFunctions::unquote(currentSection.type);
                currentSection.directives.push_back(std::move(dir));
            } else {
                currentSection.directives.push_back(std::move(dir));
            }
        }
    
        token = lexer.nextToken();
    }

    result.sections.push_back(std::move(currentSection));
    return result;
}
