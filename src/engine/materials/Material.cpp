#include "Material.hpp"

Material::Material(
    const std::string& _name, std::shared_ptr<Shader> _shader
) : name(_name), shader(_shader) {}

const std::string& Material::getName() const { return name; }
std::shared_ptr<Shader> Material::getShader() const { return shader; }
const TypedPropertyStorage& Material::getPropertyStorage() { return properties; }
const Material::TextureStorage& Material::getTextureStorage() { return textures; }
