#ifndef ENGINE_MATERIALS_MATERIAL_H_
#define ENGINE_MATERIALS_MATERIAL_H_

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

#include "TypedPropertyStorage.hpp"

class Texture;
class Shader;

template <typename T>
struct MaterialTypeTraits {
    using ArgType = const T&;
};

template <>
struct MaterialTypeTraits<Texture> {
    using ArgType = std::shared_ptr<Texture>;
};

class Material {
    using TextureStorage = std::unordered_map<
        std::string, std::shared_ptr<Texture>
    >;

    std::shared_ptr<Shader> shader;
    std::string name;
private:
    TypedPropertyStorage properties;
    TextureStorage textures;
public:
    Material(const std::string& _name, std::shared_ptr<Shader> _shader);

    template <typename T>
    Material& setProperty(
        std::string& name, typename MaterialTypeTraits<T>::ArgType value
    );

    template <typename T>
    Material& setProperty(std::string& name);

    template <typename T>
    Material& addProperty(
        std::string& name, typename MaterialTypeTraits<T>::ArgType value
    );

    template <typename T>
    Material& addProperty(std::string& name);

    template <typename T>
    const T& getProperty(std::string& name) const;

    const std::string& getName() const;
    std::shared_ptr<Shader> getShader() const;
    const TypedPropertyStorage& getPropertyStorage();
    const TextureStorage& getTextureStorage();
};

#endif // ENGINE_MATERIALS_MATERIAL_H_
