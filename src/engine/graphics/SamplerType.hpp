#ifndef ENGINE_GRAPHICS_SAMPLERTYPE_H_
#define ENGINE_GRAPHICS_SAMPLERTYPE_H_

#include "engine/resource/serialization/Serialization.hpp"

enum class SamplerType {
    Texture2D,
    CubeMap2D,
};

GENERATE_ENUM_SERIALIZATION(SamplerType,
    ENUM_OPTION(SamplerType::Texture2D, "texture2D"),
    ENUM_OPTION(SamplerType::CubeMap2D, "cubemap2D")
)

#endif // ENGINE_GRAPHICS_SAMPLERTYPE_H_
