#include "Material.hpp"

Material::Material(
    const std::string& _name,
    std::shared_ptr<Shader> _shader,
    MaterialGraphicsConfig _cfg,
    MaterialLayout _layout,
    PropertyDataStorage&& _storage,
    std::unordered_map<std::string, size_t>&& _samplerIndexes,
    std::vector<SamplerDefinition>&& _samplerDefinitions
) : name(_name), shader(_shader), cfg(_cfg), layout(std::move(_layout)), 
    samplersIndexes(_samplerIndexes), samplerDefinitions(_samplerDefinitions),
    defaultValues(std::move(_storage)) {}

Material::Material(Material&& other) noexcept
  : name(std::move(other.name)),
    shader(std::move(other.shader)),
    cfg(std::move(other.cfg)),
    layout(std::move(other.layout)),
    defaultValues(std::move(other.defaultValues)),
    samplersIndexes(std::move(other.samplersIndexes)),
    samplerDefinitions(std::move(other.samplerDefinitions)) {}

Material& Material::operator=(Material&& other) noexcept {
    if (this != &other) {
        name = std::move(other.name);
        shader = std::move(other.shader);
        cfg = std::move(other.cfg);
        layout = std::move(other.layout);
        defaultValues = std::move(other.defaultValues);
        samplersIndexes = std::move(other.samplersIndexes);
        samplerDefinitions = std::move(other.samplerDefinitions);
    }
    return *this;
}

bool Material::hasProperty(const std::string& name) const {
    return layout.hasProperty(name);
}

bool Material::hasDefaultValue(const std::string& name) const {
    return hasProperty(name);
}

bool Material::hasSampler(const std::string& name) const {
    return samplersIndexes.contains(name);
}

const SamplerDefinition& Material::getSampler(const std::string& name) const {
    size_t index = samplersIndexes.at(name);
    return samplerDefinitions[index];
}

MaterialLayout::PropertyType Material::getPropertyType(
    const std::string& name
) const {
    return layout.getPropertyInfo(name).type;
}
