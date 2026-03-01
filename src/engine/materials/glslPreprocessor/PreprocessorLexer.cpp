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

PreprocessorLexer::Token PreprocessorLexer::nextToken() {
    if (cursorPosition >= source.size()) {
        return {
            .type = PreprocessorLexer::TokenType::EndOfFile,
            .value = "",
            .line = line,
            .column = column,
            .position = cursorPosition
        };
    }

    size_t startLine = line;
    size_t startColumn = column;
    size_t startPos = cursorPosition;
    
}
