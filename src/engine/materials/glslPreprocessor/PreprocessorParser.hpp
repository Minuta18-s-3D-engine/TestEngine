#ifndef ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSORPARSER_H_
#define ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSORPARSER_H_

#include <vector>
#include <string>
#include <format>

#include "../../stringProcessing/utils/StringInfo.hpp"
#include "PreprocessorLexer.hpp"
#include "ShaderSectionType.hpp"
#include "../../project/VirtualPath.hpp"
#include "../../utils/exc/ParseExceptions.hpp"
#include "../../utils/exc/GeneralExceptions.hpp"
#include "../../utils/EnumMapper.hpp"
#include "../../stringProcessing/utils/StringFunctions.hpp"

class PreprocessorParser {
public:
    enum class ArgType {
        String, 
        Number, 
        Identifier,
        Bool,
    };
    const EnumMapper<ArgType> argTypeMapper = {
        { ArgType::String, "string" },
        { ArgType::Number, "number" },
        { ArgType::Identifier, "identifier" },
        { ArgType::Bool, "bool" }
    }; 

    struct DirectiveArg {
        ArgType type;
        std::string_view value;
        StringPos position;
    };

    struct Directive {
        std::vector<std::string_view> name;
        std::vector<PreprocessorLexer::Token> tokens;
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
    std::string source; 

    void makeException(
        const PreprocessorLexer::Token& token, 
        const std::string& message
    ) const;

    void exceptToken(
        const PreprocessorLexer::Token& token,
        PreprocessorLexer::TokenType exceptedType,
        const std::string& what
    ) const;

    void notExceptToken(
        const PreprocessorLexer::Token& token,
        PreprocessorLexer::TokenType notExceptedType,
        const std::string& what
    ) const;

    DirectiveArg requireArg(
        const Directive& d, 
        size_t index, 
        ArgType exceptedType,
        size_t exceptedArgumentsCount
    ) const;

    Directive parseDirective(
        PreprocessorLexer& lexer,
        const std::string& source,
        const PreprocessorLexer::Token& directiveToken
    );

    std::string constructDirectiveName(const Directive& d) const;
public:
    PreprocessorParser(const std::string& _source);

    ParseResult parse();
};

#endif // ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSORPARSER_H_
