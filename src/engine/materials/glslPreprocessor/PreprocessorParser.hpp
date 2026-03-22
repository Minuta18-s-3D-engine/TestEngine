#ifndef ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSORPARSER_H_
#define ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSORPARSER_H_

#include <vector>
#include <string>

#include "../../stringProcessing/utils/StringInfo.hpp"
#include "PreprocessorLexer.hpp"
#include "ShaderSectionType.hpp"
#include "../../project/VirtualPath.hpp"

class PreprocessorParser {
public:
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
        std::string source;
        std::string code;
        std::vector<SectionBlock> sections;
    };
private:
    PreprocessorLexer lexer;
    std::string_view source; 

    void makeException(
        const PreprocessorLexer::Token& token, 
        const std::string& message
    ) const;
    void exceptToken(
        const PreprocessorLexer::Token& token,
        PreprocessorLexer::TokenType exceptedType,
        const std::string& what
    ) const;
    std::vector<DirectiveArg> parseDirectiveArgs(
        PreprocessorLexer& lexer,
        const std::string& source,
        const PreprocessorLexer::Token& directiveToken
    );
    DirectiveArg requireArg(
        const Directive& d, 
        size_t index, 
        ArgType exceptedType
    ) const;
public:
    PreprocessorParser(const std::string& _source);

    ParseResult parse();
};

#endif // ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSORPARSER_H_
