#ifndef ENGINE_GRAPHICS_SHADERLAYOUT_HPP
#define ENGINE_GRAPHICS_SHADERLAYOUT_HPP

#include <unordered_map>
#include <string>
#include <vector>

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
    };

    struct PropertyInfo {
        PropertyType type;
        size_t offset;
        size_t size;
    };
private:
    std::unordered_map<std::string, PropertyInfo> properties;
    std::vector<std::string> propertyOrder;
    size_t currentOffset = 0;
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

#endif // ENGINE_GRAPHICS_SHADERLAYOUT_HPP
