#include "MaterialInstance.hpp"

MaterialInstance::MaterialInstance(
    const std::string& _name,
    std::shared_ptr<Shader> _shader,
    std::shared_ptr<Material> _material
) : shader(_shader), material(_material), name(_name) {}

bool MaterialInstance::hasProperty(const std::string& name) const {
    return propertyOverrides.hasProperty(name) || material->hasProperty(name);
}

const TypedPropertyStorage& MaterialInstance::getPropertyStorage() const {
    return propertyOverrides;
}

const Material::TextureStorage& MaterialInstance::getTextureStorage() const {
    return textureOverrides;
}
