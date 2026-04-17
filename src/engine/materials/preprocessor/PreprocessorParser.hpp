#ifndef ENGINE_MATERIALS_PREPROCESSOR_PREPROCESSORPARSER_H_
#define ENGINE_MATERIALS_PREPROCESSOR_PREPROCESSORPARSER_H_

#include <vector>
#include <string>
#include <iostream>
#include <format>
#include <set>

#include "../../stringProcessing/utils/StringInfo.hpp"
#include "PreprocessorLexer.hpp"
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

        bool nameMatch(const std::vector<std::string>& _name) {
            if (name.size() != _name.size()) return false;
            for (size_t i = 0; i < _name.size(); ++i) {
                if (name[i] != _name[i]) return false;
            }
            return true;
        }

        std::string constructDirectiveName() const {
            if (name.size() < 1) return "";
            std::string directiveName = std::string(name[0]);
            for (size_t i = 1; i < name.size(); ++i) {
                directiveName += "." + std::string(name[i]);
            }

            return directiveName;
        }
    };

    struct DirectiveSpec {
        std::vector<ArgType> argTypes;

        DirectiveSpec(
            const std::vector<ArgType>& _argTypes
        ) : argTypes(_argTypes) {}

        DirectiveSpec(
            const std::initializer_list<ArgType>& _argTypes
        ) : argTypes(_argTypes) {}

        DirectiveSpec(
            const ArgType& _argTypes
        ) : argTypes({_argTypes}) {}

        DirectiveSpec() = default; 
    };

    struct Warning {
        std::string message;
        StringPos position;
    };

    struct ParseResult {
        std::string source;
        std::string code;
        std::vector<Directive> directives;
        std::vector<Warning> warnings;
    };
private:
    std::string source; 

    std::unordered_map<std::string, DirectiveSpec> validators;
    std::set<std::string> builtins;

    void makeException(
        const PreprocessorLexer::Token& token, 
        const std::string& message
    ) const;

    void exceptToken(
        const PreprocessorLexer::Token& token,
        PreprocessorLexer::TokenType expectedType,
        const std::string& what
    ) const;

    void notExceptToken(
        const PreprocessorLexer::Token& token,
        PreprocessorLexer::TokenType notExpectedType,
        const std::string& what
    ) const;

    DirectiveArg requireArg(
        const Directive& d, 
        size_t index, 
        ArgType expectedType,
        size_t exceptedArgumentsCount
    ) const;

    Directive parseDirective(
        PreprocessorLexer& lexer,
        const PreprocessorLexer::Token directiveToken,
        PreprocessorLexer::Token& currentToken
    );

    std::optional<Warning> validateDirective(const Directive& d) const;
public:
    PreprocessorParser(const std::string& _source);

    ParseResult parse();
    void addDirectiveValidator(const std::string& name, DirectiveSpec spec);
};

#endif // ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSORPARSER_H_
