#ifndef ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSORPARSER_H_
#define ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSORPARSER_H_

#include <vector>
#include <string>

#include "PreprocessorLexer.hpp"
#include "DirectiveHandler.hpp"
#include "PreprocessorParserContext.hpp"

class PreprocessorParser {
    PreprocessorLexer lexer;
    PreprocessorParserContext parseContext;
    
    std::unordered_map<std::string, DirectiveHandler> handlers;
public:
    PreprocessorParser(const std::string& source);

    std::string applyDirectives();
};

#endif // ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSORPARSER_H_
