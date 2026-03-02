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

bool PreprocessorLexer::isAllowedInName(char c) {
    if (c >= 'a' && c <= 'z') return true;
    if (c >= 'A' && c <= 'z') return true;
    if (c >= '0' && c <= '9') return true;
    if ((c == '_') || (c == '.')) return true;
    return false;
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
    
    if (inDirective) {
        while (in(peek(), spaces)) {
            advance();
        }

        if (in(peek(), breakCharacters)) {
            if (peek() != '\0') advance();
            inDirective = false;
            return nextToken();
        }

        if (isAllowedInName(peek())) {
            size_t contentStart = cursorPosition;
            while (isAllowedInName(peek())) advance();
            return {
                .type = PreprocessorLexer::TokenType::Directive,
                .value = source.substr(
                    contentStart, cursorPosition - contentStart),
                .line = startLine,
                .column = startColumn,
                .position = startPos
            };
        }

        if (in(peek(), separators)) {
            if (peek() == '\"') {
                advance(); 
                size_t contentStart = cursorPosition;
                while (!in(peek(), "\"\n\0")) {
                    advance();
                }
                std::string_view value = source.substr(
                    contentStart, cursorPosition - contentStart
                );
                if (peek() == '\"') advance();
                
                return { 
                    .type = PreprocessorLexer::TokenType::Argument, 
                    .value = value, 
                    .line = startLine, 
                    .column = startColumn, 
                    .position = startPos 
                };
            }
            advance();
            return nextToken();
        }
    }

    if (source.substr(cursorPosition).starts_with(directiveMarker)) {
        cursorPosition += directiveMarker.size();
        column += directiveMarker.size();
        inDirective = true;
        return { 
            .type = PreprocessorLexer::TokenType::DirectiveMarker, 
            .value = directiveMarker, 
            .line = startLine, 
            .column = startColumn, 
            .position = startPos 
        };
    }

    while (cursorPosition < source.size()) {
        if (source.substr(cursorPosition).starts_with(directiveMarker)) {
            break;
        }
        advance();
    }

    return {
        .type = PreprocessorLexer::TokenType::Code,
        .value = source.substr(startPos, cursorPosition - startPos),
        .line = startLine,
        .column = startColumn,
        .position = startPos
    };
}
