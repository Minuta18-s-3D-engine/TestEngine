#ifndef ENGINE_MATERIALS_MATERIAL_H_
#define ENGINE_MATERIALS_MATERIAL_H_

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

#include "TypedPropertyStorage.hpp"

class Texture;

class Material {
public:
    using TextureStorage = std::unordered_map<
        std::string, std::shared_ptr<Texture>
    >;
private:
    std::string name;
    
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

    bool hasProperty(const std::string& name) const;
    bool isPropertySet(const std::string& name) const;

    Material& setTexture(
        const std::string& name, std::shared_ptr<Texture> _tex
    );
    Material& addTexture(
        const std::string& name, std::shared_ptr<Texture> _tex
    );
    Material& setTexture(const std::string& name);
    Material& addTexture(const std::string& name);
    std::shared_ptr<Texture> getTexture(const std::string& name);
    bool hasTexture(const std::string& name) const;
    bool isTextureSet(const std::string& name) const;

    const std::string& getName() const { return name; }

    const TypedPropertyStorage& getPropertyStorage();
    const TextureStorage& getTextureStorage();
};

#endif // ENGINE_MATERIALS_MATERIAL_H_
