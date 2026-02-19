#include "Material.hpp"

Material::Material(
    const std::string& _name
) : name(_name) {}

Material& Material::setTexture(
    const std::string& name, std::shared_ptr<Texture> _tex
) {
    textures[name] = _tex;
    return *this;
}

Material& Material::setTexture(const std::string& name) {
    textures[name] = nullptr;
    return *this;
}

Material& Material::addTexture(
    const std::string& name, std::shared_ptr<Texture> _tex
) {
    return this->setTexture(name, _tex);   
}

Material& Material::addTexture(const std::string& name) {
    return this->setTexture(name);   
}

std::shared_ptr<Texture> Material::getTexture(const std::string& name) {
    if (!textures.contains(name)) {
        throw std::invalid_argument("No such property: " + name);
    }
    if (textures[name] == nullptr) {
        throw std::invalid_argument("Property is unset: " + name);
    }
    return textures[name];
}

const TypedPropertyStorage& Material::getPropertyStorage() { return properties; }
const Material::TextureStorage& Material::getTextureStorage() { return textures; }
