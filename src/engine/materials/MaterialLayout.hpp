#ifndef ENGINE_MATERIALS_MATERIALLAYOUT_H_
#define ENGINE_MATERIALS_MATERIALLAYOUT_H_

#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <exception>
#include <stdexcept>

#define PROPERTY_TYPE_LIST \
    X(Int,    int32_t,    "int"     ) \
    X(Uint,   uint32_t,   "uint"    ) \
    X(Uint64, uint64_t,   "uint64_t") \ 
    X(Int64,  int64_t,    "int64_t" ) \ 
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

class MaterialLayout {
public:
    enum class PropertyType {
        #define X(name, type, glslType) name,
            PROPERTY_TYPE_LIST
        #undef X
        Unknown
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

    static std::string getGLSLType(PropertyType type);
    static size_t getStd430Alignment(PropertyType type);
    static size_t getStd430Size(PropertyType type); 


    struct Property {
        PropertyType type;

        size_t offset;
        size_t size;
    };
private:
    using SortEntry = std::pair<std::size_t, const std::string&>; 

    std::unordered_map<std::string, Property> properties;
    std::vector<std::string> propsOrder;

    size_t materialSize = 0;
    size_t maxAlignment;  
    bool finalized = false;

    void repackData();
public:
    template <typename T>
    void addProperty(const std::string& name);
    bool hasProperty(const std::string& name) const;

    const Property& getPropertyInfo(const std::string& name) const;
    size_t getLayoutSize() const;

    void finalize();
    bool isFinalized() const;

    std::vector<std::string> getPropertyOrder() const;
};

template <typename T>
void MaterialLayout::addProperty(const std::string& name) {
    if (finalized) return;

    PropertyType type = getPropertyType<T>();

    properties[name] = {
        .type = type,
    };
}

#endif // ENGINE_MATERIALS_MATERIALLAYOUT_H_
