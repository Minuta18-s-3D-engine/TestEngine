#ifndef ENGINE_STRINGPROCESSING_TEMPLATEENGINE_TEMPLATELEXER_HPP
#define ENGINE_STRINGPROCESSING_TEMPLATEENGINE_TEMPLATELEXER_HPP

#include <string>
#include <vector>

enum class TemplateTokenType {
    Text,
    VariableTagStart,
    VariableTagEnd,
    CommentTagStart,
    CommentTagEnd,
    TagContent,
    EndOfFile,
    Error,
};

struct TemplateToken {
    TemplateTokenType type;
    std::string_view value;
    size_t line;
    size_t column;
    size_t position;
};

class TemplateLexer {
public:
    struct Tag {
        std::string_view start;
        std::string_view end;
        TemplateTokenType startType;
        TemplateTokenType endType;
    };
private:
    std::vector<Tag> tags = {
        { "{{", "}}", TemplateTokenType::VariableTagStart, TemplateTokenType::VariableTagEnd },
        { "{#", "#}", TemplateTokenType::CommentTagStart, TemplateTokenType::CommentTagEnd },
    };

    std::string_view source;
    size_t cursorPosition = 0;
    size_t line = 0;
    size_t column = 0;

    char peek() const;
    void advance();
public:
    TemplateLexer(std::string_view source) : source(source) {}

    TemplateToken nextToken();
};

#endif // ENGINE_STRINGPROCESSING_TEMPLATEENGINE_TEMPLATELEXER_HPP
