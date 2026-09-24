#include "Material.hpp"

Material::Material(
    MaterialGraphicsConfig& config_,
    ResourceHandle<Shader> shader_,
    PropertyDataStorage&& defaultValues_,
    SamplerDefaults&& defaultSamplers_
) : config(&config_),
    shader(shader_),
    properties(std::move(defaultValues_)),
    samplers(std::move(defaultSamplers_)) {}

Material::Material(Material&& other) noexcept
  : config(other.config),
    shader(other.shader),
    properties(std::move(other.properties)),
    samplers(std::move(other.samplers)) {
}

Material& Material::operator=(Material&& other) noexcept {
    if (this != &other) {
        config = other.config;
        shader = other.shader;
        properties = std::move(other.properties);
        samplers = std::move(other.samplers);
    }
    return *this;
}

bool Material::hasProperty(const std::string& propertyName) const {
    return properties.hasProperty(propertyName);
}

bool Material::hasSampler(const std::string& samplerName) const {
    return samplers.contains(samplerName);
}

ResourceHandle<Texture> Material::getSampler(
    const std::string& samplerName
) const {
    return samplers.at(samplerName);
}

void Material::setSampler(
    const std::string &samplerName, ResourceHandle<Texture> value
) {
    samplers[samplerName] = value;
}
