#ifndef ENGINE_MATERIALS_GLSLPREPROCESSOR_SHADERFILE_H_
#define ENGINE_MATERIALS_GLSLPREPROCESSOR_SHADERFILE_H_

#include <string>
#include <unordered_map>

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

struct ShaderFileSection {
    ShaderSectionType type;
    std::string_view source;
};

struct ShaderFile {
    std::string source;
    std::unordered_map<ShaderSectionType, ShaderFileSection> sections;
};

#endif // ENGINE_MATERIALS_GLSLPREPROCESSOR_SHADERFILE_H_
