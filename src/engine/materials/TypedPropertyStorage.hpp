#ifndef ENGINE_MATERIALS_TYPEDMATERIALSTORAGE_H_
#define ENGINE_MATERIALS_TYPEDMATERIALSTORAGE_H_

#include "glm/glm.hpp"

#include <stdint.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <cstring>
#include <stdexcept>

#include "../graphics/Texture.hpp"

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

    static constexpr size_t getStd140Alignment(PropertyType type);
    static constexpr size_t getAlignment(PropertyType type);

    struct Property {
        PropertyType type;
        bool isDefaultDataSet;
        size_t defaultDataStart;
        size_t defaultDataSize;
    };
private:
    std::unordered_map<std::string, Property> properties;

    // Data now stores not just packed default data, but UBO. UBO requires to 
    // use std140 alignment rules. Alignment offset is calculated using this 
    // formula:
    //     alignedOffset = (dataSize + alignment - 1) & ~(alignment - 1); 
    // Another important rule is that UBO size must be multiple of 16.
    uint8_t* data = nullptr;
    size_t dataSize = 0;

    void reallocData(size_t n);
    void freeData();

    template <typename T>
    void createProperty(
        const std::string& name, 
        const T& value, 
        bool valueSet = true
    ) {
        PropertyType type = getPropertyType<T>();
        size_t alignment = getAlignment(type);

        size_t alignedOffset = (dataSize + alignment - 1) & ~(alignment - 1);
        size_t valueSize = sizeof(T);

        reallocData(alignedOffset + valueSize);
        properties[name] = { type, valueSet, alignedOffset, valueSize };

        if (valueSet) {
            std::memcpy(data + alignedOffset, &value, valueSize);
        }

        dataSize = alignedOffset + valueSize;

        size_t padding = (dataSize + 15) & ~15;
        if (padding > dataSize) {
            reallocData(padding);
            dataSize = padding;
        }
    }

    template <typename T>
    void createProperty(const std::string& name) {
        createProperty<T>(name, T(), false);
    }
public:
    TypedPropertyStorage();
    ~TypedPropertyStorage();
    TypedPropertyStorage(const TypedPropertyStorage& other);
    TypedPropertyStorage(TypedPropertyStorage&& other) noexcept;

    TypedPropertyStorage& operator=(const TypedPropertyStorage& other);
    TypedPropertyStorage& operator=(TypedPropertyStorage&& other) noexcept;

    template <typename T>
    void setProperty(const std::string& name, const T& value) {
        if (!hasProperty(name)) {
            createProperty<T>(name, value);
            return;
        } 

        if (!(properties[name].type == getPropertyType<T>())) {
            throw std::invalid_argument("Dynamic type change is unsupported.");
        }

        const Property& p = properties[name];
        std::memcpy(data + p.defaultDataStart, &value, sizeof(T));
    }

    template <typename T>
    void setProperty(const std::string& name) {
        if (!hasProperty(name)) {
            createProperty<T>(name);
            return;
        } 

        if (!(properties[name].type == getPropertyType<T>())) {
            throw std::invalid_argument("Dynamic type change is unsupported.");
        }

        const Property& p = properties[name];
        p.isDefaultDataSet = false;
    }

    template <typename T>
    const T& getProperty(const std::string& name) {
        if (!hasProperty(name)) { 
            throw std::invalid_argument("No such property: " + name);
        }
        if (!isPropertySet(name)) {
            throw std::invalid_argument("Property is unset: " + name);
        }
        const Property& p = properties[name];
        if (!(p.type == getPropertyType<T>())) {
            throw std::invalid_argument(
                "Property " + name + " type does not match."
            );
        }
        return *reinterpret_cast<const T*>(data + p.defaultDataStart);
    }

    bool hasProperty(const std::string& name) const;
    bool isPropertySet(const std::string& name) const;
};

#endif // ENGINE_MATERIALS_TYPEDMATERIALSTORAGE_H_
