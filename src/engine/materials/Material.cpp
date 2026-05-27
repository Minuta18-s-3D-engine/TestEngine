#include "Material.hpp"

Material::Material(
    MaterialDescriptor&& _descriptor,
    MaterialDescriptor::SamplerMap&& _samplerDefaults,
    PropertyDataStorage&& _storage
) : descriptor(_descriptor), samplerDefaults(_samplerDefaults), 
    shader(nullptr), defaultValues(std::move(_storage)) {
    defaultValues.bindLayout(&this->descriptor.layout);
}

Material::Material(Material&& other) noexcept
  : descriptor(std::move(other.descriptor)),
    shader(std::move(other.shader)),
    defaultValues(std::move(other.defaultValues)),
    samplerDefaults(std::move(other.samplerDefaults)) {
    defaultValues.bindLayout(&this->descriptor.layout);
}

Material& Material::operator=(Material&& other) noexcept {
    if (this != &other) {
        descriptor = std::move(other.descriptor);
        shader = std::move(other.shader);
        defaultValues = std::move(other.defaultValues);
        samplerDefaults = std::move(other.samplerDefaults);
        defaultValues.bindLayout(&this->descriptor.layout);
    }
    return *this;
}

bool Material::hasProperty(const std::string& name) const {
    return descriptor.layout.hasProperty(name);
}

bool Material::hasDefaultValue(const std::string& name) const {
    return hasProperty(name);
}

bool Material::hasSampler(const std::string& name) const {
    return descriptor.samplerIndexes.contains(name);
}

const SamplerDefinition& Material::getSampler(const std::string& name) const {
    size_t index = descriptor.samplerIndexes.at(name);
    return descriptor.samplerDefinitions[index];
}

MaterialLayout::PropertyType Material::getPropertyType(
    const std::string& name
) const {
    return descriptor.layout.getPropertyInfo(name).type;
}

std::string Material::getName() const { 
    return descriptor.name; 
}

const MaterialGraphicsConfig& Material::getConfig() const { 
    return descriptor.config; 
}

const MaterialLayout& Material::getLayout() const { 
    return descriptor.layout; 
}

const PropertyDataStorage& Material::getDefaultValues() const { 
    return defaultValues; 
}

const std::vector<SamplerDefinition>& Material::getSamplerDefinitions() const {
    return descriptor.samplerDefinitions;
}

const MaterialDescriptor::SamplerMap& Material::getSamplerDefaults() const { 
    return samplerDefaults; 
}

const MaterialDescriptor& Material::getDescriptor() const {
    return descriptor;
}
