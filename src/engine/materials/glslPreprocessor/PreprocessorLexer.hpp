#ifndef ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSORLEXER_H_
#define ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSORLEXER_H_

#include <string>
#include <vector>
#include <regex>

class PreprocessorLexer {
public:
    enum class TokenType {
        Number,
        String,
        Identifier,
        Dot,
        LBracket,
        RBracket,
        Comma,
        DirectiveMarker,
        Code,
        EndOfFile,
        Unknown,
    };

    struct Token {
        TokenType type;
        std::string_view value;
        size_t line;
        size_t column;
        size_t position;
    };

    enum class State {
        Code,
        Directive
    };

    struct Rule {
        TokenType type;
        std::regex pattern;
    };

    const std::string directiveMarker = "//@";

    const std::vector<Rule> directiveRules = {
        { TokenType::Number,     std::regex(R"(^(\d+\.\d+|\d+))") },
        { TokenType::String,     std::regex(R"(^"([^"\\]|\\.)*")") },
        { TokenType::Identifier, std::regex(R"(^([a-zA-Z_]\w*))") },
        { TokenType::Dot,        std::regex(R"(^\.)") },
        { TokenType::LBracket,   std::regex(R"(^\()") },
        { TokenType::RBracket,   std::regex(R"(^\))") },
        { TokenType::Comma,      std::regex(R"(^,)") } 
    };
private:
    std::string_view source;
    size_t cursorPosition = 0;
    size_t line = 0;
    size_t column = 0;

    State currentState = State::Code;
    const Token EOFToken {
        .type = PreprocessorLexer::TokenType::EndOfFile,
        .value = "",
        .line = line,
        .column = column, 
        .position = cursorPosition
    };
    const int unknownTokenSize = 1;

    char peek() const;
    void advance();
    void advance(int n);

    bool in(char c, const std::string& str);

    bool isSpace(char c);
    bool isAlpha(char c);
    bool isDigit(char c);
    bool isAlphaDigit(char c);

    PreprocessorLexer::Token handleCodeState();
    PreprocessorLexer::Token handleDirectiveState();
public:
    PreprocessorLexer(std::string_view _source) : source(_source) {}

    Token nextToken();
};

#endif // ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSORLEXER_H_
