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
        const std::string& name, typename MaterialTypeTraits<T>::ArgType value
    ) {
        if constexpr (std::is_same_v<T, Texture>) {
            textures[name] = value;
        } else {
            properties.setProperty<T>(name, value);
        }
        return *this;
    }

    template <typename T>
    Material& setProperty(const std::string& name) {
        if constexpr (std::is_same_v<T, Texture>) {
            textures[name] = nullptr;
        } else {
            properties.setProperty<T>(name);
        }
        return *this;
    }

    template <typename T>
    Material& addProperty(
        const std::string& name, typename MaterialTypeTraits<T>::ArgType value
    ) {
        return setProperty<T>(name, value);
    }

    template <typename T>
    Material& addProperty(const std::string& name) {
        return setProperty<T>(name);
    }

    template <typename T>
    const T& getProperty(const std::string& name) const {
        if constexpr (std::is_same_v<T, Texture>) {
            if (!textures.contains(name)) {
                throw std::invalid_argument("No such texture: " + name);
            }

            if (textures[name] == nullptr) {
                throw std::invalid_argument("Property is unset: " + name);
            }
            return *(textures[name]);
        } else {
            return properties.getProperty<T>(name);
        }
    }

    const std::string& getName() const;
    const std::shared_ptr<Shader> getShader() const;
    const TypedPropertyStorage& getPropertyStorage();
    const TextureStorage& getTextureStorage();
};

#endif // ENGINE_MATERIALS_MATERIAL_H_
