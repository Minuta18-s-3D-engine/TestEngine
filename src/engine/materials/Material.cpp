#include "Material.hpp"

Material::Material(
    const std::string& _name,
    MaterialGraphicsConfig _cfg,
    MaterialLayout _layout,
    PropertyDataStorage&& _storage,
    std::unordered_map<std::string, size_t>&& _samplerIndexes,
    std::vector<SamplerDefinition>&& _samplerDefinitions,
    SamplerMap&& _samplerDefaults
) : name(_name), shader(nullptr), cfg(_cfg), layout(std::move(_layout)), 
    samplersIndexes(_samplerIndexes), samplerDefinitions(_samplerDefinitions),
    samplerDefaults(_samplerDefaults),
    defaultValues(std::move(_storage)) {
    defaultValues.bindLayout(&this->layout);
}

Material::Material(Material&& other) noexcept
  : name(std::move(other.name)),
    shader(std::move(other.shader)),
    cfg(std::move(other.cfg)),
    layout(std::move(other.layout)),
    defaultValues(std::move(other.defaultValues)),
    samplersIndexes(std::move(other.samplersIndexes)),
    samplerDefinitions(std::move(other.samplerDefinitions)),
    samplerDefaults(std::move(other.samplerDefaults)) {
    defaultValues.bindLayout(&this->layout);
}

Material& Material::operator=(Material&& other) noexcept {
    if (this != &other) {
        name = std::move(other.name);
        shader = std::move(other.shader);
        cfg = std::move(other.cfg);
        layout = std::move(other.layout);
        defaultValues = std::move(other.defaultValues);
        samplersIndexes = std::move(other.samplersIndexes);
        samplerDefinitions = std::move(other.samplerDefinitions);
        samplerDefaults = std::move(other.samplerDefaults);
        defaultValues.bindLayout(&this->layout);
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
