#include "Material.hpp"

Material::Material(
    const std::string& _name
) : name(_name) {}

bool Material::hasProperty(const std::string& name) const {
    return properties.hasProperty(name);
}

bool Material::isPropertySet(const std::string& name) const {
    return properties.isPropertySet(name);
}

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
        throw std::invalid_argument("No such texture: " + name);
    }
    if (textures[name] == nullptr) {
        throw std::invalid_argument("Texture is unset: " + name);
    }
    return textures[name];
}

bool Material::hasTexture(const std::string& name) const {
    return textures.contains(name);
}

bool Material::isTextureSet(const std::string& name) const {
    if (!hasTexture(name)) {
        throw std::invalid_argument("No such texture: " + name);
    }
    return !(textures.at(name) == nullptr);
}

const TypedPropertyStorage& Material::getPropertyStorage() { return properties; }
const Material::TextureStorage& Material::getTextureStorage() { return textures; }
