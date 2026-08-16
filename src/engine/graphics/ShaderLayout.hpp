#ifndef ENGINE_GRAPHICS_SHADERLAYOUT_HPP
#define ENGINE_GRAPHICS_SHADERLAYOUT_HPP

#include <cstdint>
#include <unordered_map>
#include <string>
#include <vector>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#define PROPERTY_TYPE_LIST \
    X(Int,    int32_t,    "int"     ) \
    X(Uint,   uint32_t,   "uint"    ) \
    X(Float,  float,      "float"   ) \
    X(Bool,   bool,       "bool"    ) \
    X(Vec2,   glm::vec2,  "vec2"    ) \
    X(IVec2,  glm::ivec2, "ivec2"   ) \
    X(UVec2,  glm::uvec2, "uvec2"   ) \
    X(Vec3,   glm::vec3,  "vec3"    ) \
    X(IVec3,  glm::ivec3, "ivec3"   ) \
    X(UVec3,  glm::uvec3, "uvec3"   ) \
    X(Vec4,   glm::vec4,  "vec4"    ) \
    X(IVec4,  glm::ivec4, "ivec4"   ) \
    X(UVec4,  glm::uvec4, "uvec4"   ) \
    X(Mat2,   glm::mat2,  "mat2"    ) \
    X(Mat3,   glm::mat3,  "mat3"    ) \
    X(Mat4,   glm::mat4,  "mat4"    ) \

class ShaderLayout {
public:
    enum class PropertyType {
        #define X(name, type, glslType) name,
            PROPERTY_TYPE_LIST
        #undef X
        Unknown
    };

    struct PropertyInfo {
        PropertyType type;
        size_t offset;
        size_t size;
    };

    template <typename T>
    static PropertyType getPropertyType() {
        using DecayedT = std::decay_t<T>;

        #define X(name, type, glslType) \
            if constexpr (std::is_same_v<DecayedT, type>) \
                return PropertyType::name;

            PROPERTY_TYPE_LIST
        #undef X

        return PropertyType::Unknown;
    }

    static size_t getSize(const PropertyType propType) {
        if (propType == PropertyType::Bool)
            return 4;

        #define X(name, type, glslType) \
            if (propType == PropertyType::name) \
                return sizeof(type);

            PROPERTY_TYPE_LIST
        #undef X

        return 0;
    }
private:
    std::unordered_map<std::string, PropertyInfo> properties;
    std::vector<std::string> propertyOrder;

    size_t layoutSize = 0;
    size_t maxAlignment = 0;

    bool finalized = false;

    void repackData();
public:
    ShaderLayout() = default;

    template <typename T>
    void addProperty(const std::string& name);
    bool hasProperty(const std::string& name) const;

    const PropertyInfo& getPropertyInfo(const std::string& name) const;
    size_t getLayoutSize() const;

    void finalize();
    bool isFinalized() const;

    std::vector<std::string> getProperties() const;
};

template <typename T>
void ShaderLayout::addProperty(const std::string& name) {
    if (finalized) return;

    const PropertyType type = getPropertyType<T>();

    properties[name] = {
        .type = type,
        .offset = 0,
        .size = 0
    };
}

#endif // ENGINE_GRAPHICS_SHADERLAYOUT_HPP
