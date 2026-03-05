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

void PreprocessorLexer::advance(int n) {
    for (int i = 0; i < n; ++i) advance();
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

PreprocessorLexer::Token PreprocessorLexer::handleCodeState() {
    std::string_view lastPart = source.substr(cursorPosition);

    size_t markerPos = lastPart.find(directiveMarker);

    Token resultingToken;
    if (markerPos == 0) {
        resultingToken = {
            .type = PreprocessorLexer::TokenType::DirectiveMarker,
            .value = directiveMarker,
            .line = line,
            .column = column, 
            .position = cursorPosition
        };
        currentState = State::Directive;
        advance(directiveMarker.size());
    } else {
        std::size_t codeLen = markerPos;
        if (markerPos == std::string_view::npos) codeLen = lastPart.size();
        resultingToken = {
            .type = PreprocessorLexer::TokenType::Code,
            .value = lastPart.substr(0, codeLen),
            .line = line,
            .column = column, 
            .position = cursorPosition
        };
        advance(codeLen);
    }

    return resultingToken;
}

PreprocessorLexer::Token PreprocessorLexer::handleDirectiveState() {
    std::string lastPart(source.substr(cursorPosition));

    for (const auto& rule : directiveRules) {
        std::smatch match;
        if (std::regex_search(lastPart, match, rule.pattern)) {
            Token resultingToken {
                .type = rule.type,
                .value = match.str(),
                .line = line,
                .column = column, 
                .position = cursorPosition
            };
            advance(match.str().size());
            return resultingToken;
        }
    }

    Token resultingToken {
        .type = PreprocessorLexer::TokenType::Unknown,
        .value = source.substr(cursorPosition, unknownTokenSize),
        .line = line,
        .column = column, 
        .position = cursorPosition
    };
    advance(unknownTokenSize);
    return resultingToken;
}

PreprocessorLexer::Token PreprocessorLexer::nextToken() {
    if (cursorPosition >= source.size()) {
        return EOFToken;
    } 

    if (currentState == State::Code) {
        return handleCodeState();
    } else if (currentState == State::Directive) {
        while (cursorPosition < source.size() && in(peek(), " \t\r")) {
            advance();
        }

        if (cursorPosition >= source.size()) {
            return EOFToken;
        }

        if (in(peek(), "\n")) {
            advance();
            currentState = State::Code;
            if (cursorPosition >= source.size()) {
                return EOFToken;
            }
            return handleCodeState();
        }

        return handleDirectiveState();
    }

    return EOFToken;
}
