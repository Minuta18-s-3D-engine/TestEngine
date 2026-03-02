#ifndef ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSORLEXER_H_
#define ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSORLEXER_H_

#include <string>
#include <vector>

class PreprocessorLexer {
public:
    enum class TokenType {
        Code,
        DirectiveMarker,
        Directive,
        Argument,
        EndOfFile
    };

    struct Token {
        TokenType type;
        std::string_view value;
        size_t line;
        size_t column;
        size_t position;
    };
private:
    const std::string directiveMarker = "//@";
    const std::string spaces = " \t";
    const std::string breakCharacters = "\n\r\0";
    const std::string separators = "\"(),";

    std::string_view source;
    size_t cursorPosition = 0;
    size_t line = 0;
    size_t column = 0;
    bool inDirective = false;

    char peek() const;
    void advance();

    bool in(char c, const std::string& str);
    bool isAllowedInName(char c);
public:
    PreprocessorLexer(std::string_view _source) : source(_source) {}

    Token nextToken();
};

#endif // ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSORLEXER_H_
