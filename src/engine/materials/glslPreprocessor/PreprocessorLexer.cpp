#include "PreprocessorLexer.hpp"

char PreprocessorLexer::peek() const {
    if (cursorPosition >= source.size()) {
        return '\0';
    }
    return source[cursorPosition];
}

void PreprocessorLexer::advance() {
    if (peek() == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    cursorPosition++;
}

bool PreprocessorLexer::in(char c, const std::string& str) {
    for (char s : str) {
        if (s == c) return true;
    }
    return false;
}

bool PreprocessorLexer::isSpace(char c) {
    return (c == ' ') || (c == '\t') || (c == '\n') || (c == '\r');
}

bool PreprocessorLexer::isAlpha(char c) {
    return ((c >= 'a') && (c <= 'z')) ||
        ((c >= 'A') && (c <= 'Z'));
}

bool PreprocessorLexer::isDigit(char c) {
    return ((c >= '0') && (c <= '9'));
}

bool PreprocessorLexer::isAlphaDigit(char c) {
    return isAlpha(c) || isDigit(c);
}

PreprocessorLexer::Token PreprocessorLexer::nextToken() {

}
