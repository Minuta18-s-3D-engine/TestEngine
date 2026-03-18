#ifndef ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSORPARSER_H_
#define ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSORPARSER_H_

#include <vector>
#include <string>

#include "../../stringProcessing/utils/StringInfo.hpp"
#include "PreprocessorLexer.hpp"
#include "ShaderSectionType.hpp"

class PreprocessorParser {
    enum class DirectiveType {
        Section,
        Include,
    };

    enum class ArgType {
        String, 
        Number, 
        Identifier,
        Bool,
    };

    struct DirectiveArg {
        ArgType type;
        std::string_view value;
        StringPos position;
    };

    struct Directive {
        DirectiveType type;
        std::string_view name;
        std::vector<DirectiveArg> args;
        StringPos position;
    };

    struct SectionBlock {
        ShaderSectionType type;
        std::string_view code;
        std::vector<Directive> directives;
    };

    struct ParseResult {
        std::string code;
        std::vector<SectionBlock> sections;
    };
private:
    PreprocessorLexer lexer;
public:
    PreprocessorParser(const std::string& source);

};

#endif // ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSORPARSER_H_
