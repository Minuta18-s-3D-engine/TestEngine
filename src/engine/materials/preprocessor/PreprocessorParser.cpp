#include "PreprocessorParser.hpp"

using IssueType = ShaderDiagnostic::IssueType;

PreprocessorParser::PreprocessorParser(
    const std::string& _source, 
    const VirtualPath& _filePath,
    ShaderDiagnostic& _diagnostic
) : source(_source), diagnostic(_diagnostic), path(_filePath) {}

void PreprocessorParser::makeException(
    const PreprocessorLexer::Token& token, 
    const std::string& message
) const {
    std::string what = "Preprocessor error at line " + 
        std::to_string(token.line) + ", col " + std::to_string(token.column) + 
        ": " + message + ".";
    diagnostic.report(IssueType::Error, path, token.line, token.column, what);
    throw ShaderDiagnostic::preprocessor_error(what, diagnostic);
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

PreprocessorParser::DirectiveArg PreprocessorParser::requireArg(
    const Directive& d, 
    size_t index, 
    ArgType expectedType,
    size_t exceptedArgumentsCount
) const {
    if (index >= d.args.size()) {
        std::string what = "Directive '" + d.constructDirectiveName() + 
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

std::vector<PreprocessorLexer::Token>
PreprocessorParser::collectDirectiveTokens(
    PreprocessorLexer& lexer,
    const PreprocessorLexer::Token& directiveToken,
    PreprocessorLexer::Token& currentToken
) {
    std::vector<PreprocessorLexer::Token> tokens;
    tokens.push_back(directiveToken);

    auto token = lexer.nextToken();
    while (
        token.type != PreprocessorLexer::TokenType::Code &&
        token.type != PreprocessorLexer::TokenType::EndOfFile
    ) {
        tokens.push_back(token);
        token = lexer.nextToken();
    }

    currentToken = token;
    return tokens;
}

size_t PreprocessorParser::parseDirectiveName(Directive& result) const {
    exceptToken(
        result.tokens[1],
        PreprocessorLexer::TokenType::Identifier,
        "directive name"
    );

    bool prevDot = false;
    size_t bracketIndex = result.tokens.size();

    for (size_t i = 1; i < result.tokens.size(); ++i) {
        const auto& token = result.tokens[i];
        if (token.type == PreprocessorLexer::TokenType::LBracket) {
            bracketIndex = i;
            if (!prevDot) {
                makeException(result.tokens[i - 1], "Invalid syntax");
            }
            break;
        }

        if (prevDot) {
            exceptToken(
                token, PreprocessorLexer::TokenType::Dot,
                "dot separator"
            );
            prevDot = false;
        } else {
            exceptToken(
                token, PreprocessorLexer::TokenType::Identifier,
                "name"
            );
            result.name.push_back(token.value);
            prevDot = true;
        }
    }

    if (bracketIndex == result.tokens.size()) {
        makeException(result.tokens[0], "Brackets not found");
    }

    return bracketIndex;
}

PreprocessorParser::DirectiveArg
PreprocessorParser::makeDirectiveArg(
    const PreprocessorLexer::Token& token
) const {
    DirectiveArg arg;
    arg.position = {
        .line = token.line,
        .col = token.column,
        .pos = token.position
    };
    arg.value = token.value;

    if (token.type == PreprocessorLexer::TokenType::Number) {
        arg.type = ArgType::Number;
    } else if (token.type == PreprocessorLexer::TokenType::String) {
        arg.type = ArgType::String;
        arg.value = StringFunctions::unquote(arg.value);
    } else if (token.type == PreprocessorLexer::TokenType::Identifier) {
        if (token.value == "true" || token.value == "false") {
            arg.type = ArgType::Bool;
        } else {
            arg.type = ArgType::Identifier;
        }
    } else {
        makeException(token, "Invalid argument type");
    }

    return arg;
}

void PreprocessorParser::parseDirectiveArgs(
    Directive& result,
    size_t bracketIndex
) const {
    bool expectArg = true;
    for (size_t i = bracketIndex + 1; i < result.tokens.size(); ++i) {
        const auto& token = result.tokens[i];

        if (token.type == PreprocessorLexer::TokenType::RBracket) {
            break;
        }

        if (expectArg) {
            result.args.push_back(makeDirectiveArg(token));
            expectArg = false;
        } else {
            if (token.type == PreprocessorLexer::TokenType::Comma) {
                expectArg = true;
            } else {
                makeException(token, "Expected ',' or ')'");
            }
        }
    }

    if (result.tokens.back().type != PreprocessorLexer::TokenType::RBracket) {
        makeException(result.tokens.back(), "Missing closing ')'");
    }
}

PreprocessorParser::Directive PreprocessorParser::parseDirective(
    PreprocessorLexer& lexer,
    const PreprocessorLexer::Token directiveToken,
    PreprocessorLexer::Token& currentToken
) {
    Directive result;
    result.position = {
        .line = directiveToken.line,
        .col = directiveToken.column,
        .pos = directiveToken.position
    };

    result.tokens = collectDirectiveTokens(
        lexer, directiveToken, currentToken
    );
    if (result.tokens.size() < 2) {
        makeException(directiveToken, "Syntax error");
    }

    size_t bracketIndex = parseDirectiveName(result);
    parseDirectiveArgs(result, bracketIndex);
    return result;
}

std::optional<PreprocessorParser::Warning> 
PreprocessorParser::validateDirective(
    const Directive& d
) const {
    const std::string key = d.constructDirectiveName();

    if (!validators.contains(key)) {
        return Warning{
            .message = "Unknown directive '" + key + "'",
            .position = d.position
        };
    }

    const auto& expected = validators.at(key).argTypes;

    if (d.args.size() != expected.size()) {
        makeException(
            d.tokens[0],
            "Directive '" + key + "' expects " +
            std::to_string(expected.size()) +
            " argument" + (expected.size() > 1 ? "s" : "") + ", found " + 
            std::to_string(d.args.size())
        );
    }

    for (size_t i = 0; i < expected.size(); ++i) {
        if (d.args[i].type != expected[i]) {
            makeException(
                d.tokens[0],
                "Directive '" + key + "', argument " +
                std::to_string(i + 1) + ": expected type " +
                argTypeMapper.toString(expected[i]) +
                ", found type " +
                argTypeMapper.toString(d.args[i].type)
            );
        }
    }

    return std::nullopt;
}

PreprocessorParser::ParseResult PreprocessorParser::parse() {
    PreprocessorParser::ParseResult result;
    result.source = this->source;
    PreprocessorLexer lexer(result.source);

    auto token = lexer.nextToken();

    while (token.type != PreprocessorLexer::TokenType::EndOfFile) {
        if (token.type == PreprocessorLexer::TokenType::Code) {
            result.code += std::string(token.value);
        } else if (
            token.type == PreprocessorLexer::TokenType::DirectiveMarker
        ) {
            Directive dir = parseDirective(lexer, token, token);

            if (auto warn = validateDirective(dir)) {
                result.warnings.push_back(std::move(*warn));
            }

            result.directives.push_back(std::move(dir));
            continue;
        }
    
        token = lexer.nextToken();
    }

    return result;
}

void PreprocessorParser::addDirectiveValidator(
    const std::string& name, DirectiveSpec spec
) {
    if (builtins.contains(name)) {
        diagnostic.report(
            IssueType::Critical, "Can't override built-in directive"
        );
    }
    validators[name] = spec;
}
