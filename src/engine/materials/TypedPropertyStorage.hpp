#ifndef ENGINE_MATERIALS_TYPEDMATERIALSTORAGE_H_
#define ENGINE_MATERIALS_TYPEDMATERIALSTORAGE_H_

#include "glm/glm.hpp"

#include <stdint.h>
#include <vector>
#include <string>
#include <unordered_map>

#define PROPERTY_TYPE_LIST \
    X(Int,   int32_t)      \
    X(Uint,  uint32_t)     \
    X(Float, float)        \
    X(Bool,  bool)         \
    X(Vec2,  glm::vec2)    \
    X(IVec2, glm::ivec2)   \
    X(UVec2, glm::uvec2)   \
    X(Vec3,  glm::vec3)    \
    X(IVec3, glm::ivec3)   \
    X(UVec3, glm::uvec3)   \
    X(Vec4,  glm::vec4)    \
    X(IVec4, glm::ivec4)   \
    X(UVec4, glm::uvec4)   \
    X(Mat2,  glm::mat2)    \
    X(Mat3,  glm::mat3)    \
    X(Mat4,  glm::mat4)    \

class TypedPropertyStorage {
public:
    enum class PropertyType {
        #define X(name, type) name,
            PROPERTY_TYPE_LIST
        #undef X
        Unknown
    };

    template <typename T>
    static constexpr PropertyType getPropertyType() {
        using DecayedT = std::decay_t<T>;

        #define X(name, type) \
            if constexpr (std::is_same_v<DecayedT, type>) \ 
                return PropertyType::name;

            PROPERTY_TYPE_LIST
        #undef X

        return PropertyType::Unknown;
    }

    struct Property {
        PropertyType type;
        bool isDefaultDataSet;
        size_t defaultDataStart;
        size_t defaultDataSize;
    };
private:
    std::unordered_map<std::string, Property> properties;

    uint8_t* data;

    void allocData();
    void reallocData(int n);
    void freeData();
public:
    TypedPropertyStorage();
    ~TypedPropertyStorage();

    template <typename T>
    void setProperty(const std::string& name, const T& value);

    template <typename T>
    const T& getProperty(const std::string& name);
};

#endif // ENGINE_MATERIALS_TYPEDMATERIALSTORAGE_H_
