#ifndef ENGINE_MATERIALS_MATERIALLAYOUT_H_
#define ENGINE_MATERIALS_MATERIALLAYOUT_H_

#include <string>
#include <unordered_map>

#define PROPERTY_TYPE_LIST \
    X(Int,   int32_t,    "int")     \
    X(Uint,  uint32_t,   "uint")    \
    X(Float, float,      "float")   \
    X(Bool,  bool,       "bool")    \
    X(Vec2,  glm::vec2,  "vec2")    \
    X(IVec2, glm::ivec2, "ivec2")   \
    X(UVec2, glm::uvec2, "uvec2")   \
    X(Vec3,  glm::vec3,  "vec3")    \
    X(IVec3, glm::ivec3, "ivec3")   \
    X(UVec3, glm::uvec3, "uvec3")   \
    X(Vec4,  glm::vec4,  "vec4")    \
    X(IVec4, glm::ivec4, "ivec4")   \
    X(UVec4, glm::uvec4, "uvec4")   \
    X(Mat2,  glm::mat2,  "mat2")    \
    X(Mat3,  glm::mat3,  "mat3")    \
    X(Mat4,  glm::mat4,  "mat4")    \

class MaterialLayout {
public:
    enum class PropertyType {
        #define X(name, type, glslType) name,
            PROPERTY_TYPE_LIST
        #undef X
        Unknown
    };

    template <typename T>
    static constexpr PropertyType getPropertyType() {
        using DecayedT = std::decay_t<T>;

        #define X(name, type, glslType) \
            if constexpr (std::is_same_v<DecayedT, type>) \
                return PropertyType::name;

            PROPERTY_TYPE_LIST
        #undef X

        return PropertyType::Unknown;
    }

    static constexpr std::string getGLSLType(PropertyType t) {
        #define X(name, type, glslType) \
            if (t == PropertyType::name) return glslType ;

            PROPERTY_TYPE_LIST
        #undef X
        return "unknown";
    }

    static constexpr size_t getStd430Alignment(PropertyType type);
    
    struct Property {
        PropertyType type;
        bool isDefaultDataSet;
    };
private:
    std::unordered_map<std::string, Property> properties;
public:
    template <typename T>
    void setProperty(const std::string& name);

    template <typename T>
    void setProperty(const std::string& name, const T& value);

    template <typename T>
    const T& getProperty(const std::string& name) const;

    bool hasProperty(const std::string& name) const;
    bool isPropertySet(const std::string& name) const;
};

#endif // ENGINE_MATERIALS_MATERIALLAYOUT_H_
