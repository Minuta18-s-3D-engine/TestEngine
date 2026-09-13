#ifndef ENGINE_GRAPHICS_SHADERPROPERTYTYPES_HPP
#define ENGINE_GRAPHICS_SHADERPROPERTYTYPES_HPP

#include <cstdint>
#include <array>
#include <glm/glm.hpp>

namespace shader_layout {

enum class PropertyType : uint8_t {
    Float, Int, Uint, Bool,
    Vec2, IVec2, UVec2,
    Vec3, IVec3, UVec3,
    Vec4, IVec4, UVec4,
    Mat2, Mat3, Mat4, Count
};

enum class SamplerType : uint8_t {
    Texture2D,
    CubeMap2D,
    Count
};

struct PropertyTypeInfo {
    const char* glslName;
    uint32_t std430Size;
    uint32_t std430Alignment;
    uint32_t cppSize;
};

inline constexpr std::array<PropertyTypeInfo,
    static_cast<size_t>(PropertyType::Count)> kPropertyTypeTable = {{
        // glsl      std430  align  cppSize
        {  "float",  4,      4,     4  },
        {  "int",    4,      4,     4  },
        {  "uint",   4,      4,     4  },
        {  "bool",   4,      4,     4  },
        {  "vec2",   8,      8,     8  },
        {  "ivec2",  8,      8,     8  },
        {  "uvec2",  8,      8,     8  },
        {  "vec3",   12,     16,    12 },
        {  "ivec3",  12,     16,    12 },
        {  "uvec3",  12,     16,    12 },
        {  "vec4",   16,     16,    16 },
        {  "ivec4",  16,     16,    16 },
        {  "uvec4",  16,     16,    16 },
        {  "mat2",   16,     8,     16 },
        {  "mat3",   48,     16,    36 },
        {  "mat4",   64,     16,    64 },
    }};

constexpr const PropertyTypeInfo& typeInfo(PropertyType t) noexcept {
    return kPropertyTypeTable[static_cast<size_t>(t)];
}

template <typename T> struct PropertyTypeOf {
    static_assert(sizeof(T) == 0,
        "Unsupported property type; add specialization");
};

#define SHADER_LAYOUT_REGISTER(CPP, ENUM) \
template <> struct PropertyTypeOf<CPP> { \
    static constexpr PropertyType value = PropertyType::ENUM; }

SHADER_LAYOUT_REGISTER(float,          Float);
SHADER_LAYOUT_REGISTER(int32_t,        Int);
SHADER_LAYOUT_REGISTER(uint32_t,       Uint);
SHADER_LAYOUT_REGISTER(bool,           Bool);
SHADER_LAYOUT_REGISTER(glm::vec2,      Vec2);
SHADER_LAYOUT_REGISTER(glm::ivec2,     IVec2);
SHADER_LAYOUT_REGISTER(glm::uvec2,     UVec2);
SHADER_LAYOUT_REGISTER(glm::vec3,      Vec3);
SHADER_LAYOUT_REGISTER(glm::ivec3,     IVec3);
SHADER_LAYOUT_REGISTER(glm::uvec3,     UVec3);
SHADER_LAYOUT_REGISTER(glm::vec4,      Vec4);
SHADER_LAYOUT_REGISTER(glm::ivec4,     IVec4);
SHADER_LAYOUT_REGISTER(glm::uvec4,     UVec4);
SHADER_LAYOUT_REGISTER(glm::mat2,      Mat2);
SHADER_LAYOUT_REGISTER(glm::mat3,      Mat3);
SHADER_LAYOUT_REGISTER(glm::mat4,      Mat4);
#undef SHADER_LAYOUT_REGISTER

template <typename T>
inline constexpr PropertyType propertyTypeOf =
    PropertyTypeOf<std::decay_t<T>>::value;

}

#endif // ENGINE_GRAPHICS_SHADERPROPERTYTYPES_HPP
