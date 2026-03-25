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
    PreprocessorLexer::TokenType exceptedType,
    const std::string& what
) const {
    if (token.type != exceptedType) {
        makeException(token, "Expected " + what);
    }
}

PreprocessorParser::DirectiveArg PreprocessorParser::requireArg(
    const Directive& d, 
    size_t index, 
    ArgType exceptedType,
    size_t exceptedArgumentsCount
) const {
    if (index >= d.args.size()) {
        std::string what = "Directive '" + std::string(d.name) = "' expects " +
            std::to_string(exceptedArgumentsCount) + " found " + 
            std::to_string(d.args.size());
        throw exc::parse_error(what);
    }

    const auto& arg = d.args[index];
    if (arg.type != exceptedType) {
        std::string what = "Excepted type " + 
            argTypeMapper.toString(exceptedType) + ", found type " +
            argTypeMapper.toString(arg.type);
        throw exc::parse_error(what);
    }
    return arg;
}

std::vector<PreprocessorParser::DirectiveArg> 
PreprocessorParser::parseDirectiveArgs(
    PreprocessorLexer& lexer,
    const std::string& source,
    const PreprocessorLexer::Token& directiveToken
) {
    std::vector<DirectiveArg> args;

    auto token = lexer.nextToken();
    exceptToken(
        token, 
        PreprocessorLexer::TokenType::LBracket, 
        "'(' after directive name"
    );

    token = lexer.nextToken();

    while (
        token.type != PreprocessorLexer::TokenType::RBracket &&
        token.type != PreprocessorLexer::TokenType::EndOfFile &&
        token.type != PreprocessorLexer::TokenType::Code
    ) {
        DirectiveArg arg;
        arg.position = {token.line, token.column, token.position};
        arg.value = token.value;

        switch (token.type) {
            case PreprocessorLexer::TokenType::Number:
                arg.type = ArgType::Number;
                break;
            case PreprocessorLexer::TokenType::String:
                arg.value = StringFunctions::unquote(arg.value);
                arg.type = ArgType::String;
                break;
            case PreprocessorLexer::TokenType::Identifier:
                if (token.value == "true" || token.value == "false") {
                    arg.type = ArgType::Bool;
                } else {
                    arg.type = ArgType::Identifier;
                }
                break;
            default:
                makeException(token, "Invalid argument type in directive");
        }

        args.push_back(arg);

        token = lexer.nextToken();
        if (token.type == PreprocessorLexer::TokenType::Comma) {
            token = lexer.nextToken();
            if (token.type == PreprocessorLexer::TokenType::RBracket) {
                makeException(token, "Trailing comma in directive arguments");
            }
        } else if (token.type != PreprocessorLexer::TokenType::RBracket) {
            makeException(token, "Expected ',' or ')' between arguments");
        }
    }

    if (
        token.type == PreprocessorLexer::TokenType::EndOfFile ||
        token.type == PreprocessorLexer::TokenType::Code
    ) {
        makeException(
            directiveToken, 
            "Unclosed directive arguments (expected ')')"
        );
    }
    
    return args;
}

PreprocessorParser::ParseResult PreprocessorParser::parse() {
    PreprocessorParser::ParseResult result;
    result.source = source;
    PreprocessorLexer lexer(std::string_view(
        result.source.data(),
        result.source.size()));

}
