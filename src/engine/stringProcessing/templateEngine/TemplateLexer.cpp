#include "TemplateLexer.hpp"

char TemplateLexer::peek() const {
    if (cursorPosition >= source.size()) {
        return '\0';
    }
    return source[cursorPosition];
}

void TemplateLexer::advance() {
    if (peek() == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    cursorPosition++;
}

TemplateToken TemplateLexer::nextToken() {
    if (cursorPosition >= source.size()) {
        return { 
            .type = TemplateTokenType::EndOfFile, 
            .value = "", 
            .line = line, 
            .column = column, 
            .position = cursorPosition 
        };
    }

    size_t startLine = line;
    size_t startCol = column;
    size_t startPos = cursorPosition;

    for (const auto& tag : tags) {
        if (source.substr(cursorPosition).starts_with(tag.start)) {
            for (size_t i = 0; i < tag.start.size(); ++i) advance();
            return { tag.startType, tag.start, startLine, startCol, startPos };
        }
        if (source.substr(cursorPosition).starts_with(tag.end)) {
            for (size_t i = 0; i < tag.end.size(); ++i) advance();
            return { tag.endType, tag.end, startLine, startCol, startPos };
        }
    }

    while (cursorPosition < source.size()) {
        bool hitMarker = false;
        for (const auto& tag : tags) {
            if (source.substr(cursorPosition).starts_with(tag.start) || 
                source.substr(cursorPosition).starts_with(tag.end)) {
                hitMarker = true;
                break;
            }
        }
        
        if (hitMarker) break;
        advance();
    }

    return {
        TemplateTokenType::Text,
        source.substr(startPos, cursorPosition - startPos),
        startLine,
        startCol,
        startPos
    };
}
