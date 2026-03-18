#ifndef ENGINE_MATERIALS_GLSLPREPROCESSOR_SHADERSECTIONTYPE_H_
#define ENGINE_MATERIALS_GLSLPREPROCESSOR_SHADERSECTIONTYPE_H_

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

#endif // ENGINE_MATERIALS_GLSLPREPROCESSOR_SHADERSECTIONTYPE_H_
