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

PreprocessorParser::ParseResult PreprocessorParser::parse() {
    PreprocessorParser::ParseResult result;
    result.source = source;
    PreprocessorLexer lexer(std::string_view(
        result.source.data(),
        result.source.size()));

}
