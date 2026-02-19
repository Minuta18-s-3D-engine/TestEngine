#ifndef ENGINE_MATERIALS_MATERIAL_H_
#define ENGINE_MATERIALS_MATERIAL_H_

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

#include "TypedPropertyStorage.hpp"

class Texture;

template <typename T>
struct MaterialTypeTraits {
    using ArgType = const T&;
};

class Material {
    using TextureStorage = std::unordered_map<
        std::string, std::shared_ptr<Texture>
    >;

    std::string name;
private:
    TypedPropertyStorage properties;
    TextureStorage textures;
public:
    Material(const std::string& _name);

    template <typename T>
    Material& setProperty(
        const std::string& name, const T& value
    ) {
        properties.setProperty<T>(name, value);
        return *this;
    }

    template <typename T>
    Material& setProperty(const std::string& name) {
        properties.setProperty<T>(name);
        return *this;
    }

    template <typename T>
    Material& addProperty(const std::string& name, const T& value) {
        return setProperty<T>(name, value);
    }

    template <typename T>
    Material& addProperty(const std::string& name) {
        return setProperty<T>(name);
    }

    template <typename T>
    const T& getProperty(const std::string& name) const {
        return properties.getProperty<T>(name);
    }

    Material& setTexture(const std::string& name, std::shared_ptr<Texture> _tex);
    Material& setTexture(const std::string& name);
    Material& addTexture(const std::string& name, std::shared_ptr<Texture> _tex);
    Material& addTexture(const std::string& name);
    std::shared_ptr<Texture> getTexture(const std::string& name);

    const TypedPropertyStorage& getPropertyStorage();
    const TextureStorage& getTextureStorage();
};

#endif // ENGINE_MATERIALS_MATERIAL_H_
