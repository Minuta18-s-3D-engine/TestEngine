#ifndef ENGINE_MATERIALS_MATERIAL_H_
#define ENGINE_MATERIALS_MATERIAL_H_

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

#include "TypedPropertyStorage.hpp"

class Texture;

class Material {
    using TextureStorage = std::unordered_map<std::string, std::shared_ptr<Texture>>;
private:
    TypedPropertyStorage& properties;
    TextureStorage textures;
public:
    Material() = default;

    template <typename T>
    Material& setProperty(std::string& name, const T& value);

    template <typename T>
    Material& setProperty(std::string& name);

    template <typename T>
    const T& getProperty(std::string& name);
};

#endif // ENGINE_MATERIALS_MATERIAL_H_
