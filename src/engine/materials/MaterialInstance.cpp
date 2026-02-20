#include "MaterialInstance.hpp"

MaterialInstance::MaterialInstance(
    const std::string& _name,
    std::shared_ptr<Shader> _shader,
    std::shared_ptr<Material> _material
) : shader(_shader), material(_material), name(_name) {}

void MaterialInstance::checkTextureExists(const std::string& name) const {
    if (!material->hasTexture(name)) {
        throw std::invalid_argument(
            "Texture '" + name + "' does not exist in the material."
        );
    }
}

bool MaterialInstance::hasProperty(const std::string& name) const {
    return propertyOverrides.hasProperty(name) || material->hasProperty(name);
}

MaterialInstance& MaterialInstance::setTexture(
    const std::string& name, std::shared_ptr<Texture> _tex
) {
    checkTextureExists(name);
    textureOverrides[name] = _tex;
    return *this;
}

MaterialInstance& MaterialInstance::setTexture(const std::string& name) {
    checkTextureExists(name);
    textureOverrides.erase(name);
    return *this;
}

std::shared_ptr<Texture> MaterialInstance::getTexture(const std::string& name) {
    auto it = textureOverrides.find(name);
    if (it != textureOverrides.end()) {
        return it->second;
    }
    return material->getTexture(name);
}

bool MaterialInstance::hasTexture(const std::string& name) const {
    return textureOverrides.contains(name) || material->hasTexture(name);
}

const TypedPropertyStorage& MaterialInstance::getPropertyStorage() const {
    return propertyOverrides;
}

const Material::TextureStorage& MaterialInstance::getTextureStorage() const {
    return textureOverrides;
}
