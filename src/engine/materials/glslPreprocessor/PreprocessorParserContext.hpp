#ifndef ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSORPARSERCONTEXT_H_
#define ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSORPARSERCONTEXT_H_

#include <string>
#include <vector>

#include "PreprocessorLexer.hpp"
#include "../../utils/EnumMapper.hpp"

enum class ShaderSectionType {
    GVertex,
    GFragment,
    LFragment
};
const EnumMapper<ShaderSectionType> ShaderSectionTypeMap({
    { ShaderSectionType::GVertex, "GVertex" },
    { ShaderSectionType::GFragment, "GFragment" },
    { ShaderSectionType::LFragment, "LFragment" }
});

class PreprocessorParserContext {
    std::string source;
    std::vector<PreprocessorLexer::Token> tokenizedSource;
    std::vector<PreprocessorLexer::Token&> foundDirectives; 
    ShaderSectionType type;

    int cursor = 0;
};

#endif // ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSORPARSERCONTEXT_H_
