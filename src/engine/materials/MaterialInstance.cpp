#include "MaterialInstance.hpp"

MaterialInstance::MaterialInstance(
    const std::string& _name, 
    const Material& _material, 
    const MaterialDataBuffer& _buffer
) : name(_name), baseMaterial(&_material), buffer(&_buffer), 
    properties(baseMaterial->getDefaultValues()) {
    properties.bindLayout(&baseMaterial->getLayout());
}

MaterialInstance::MaterialInstance(const MaterialInstance& other)
  : name(other.name),
    baseMaterial(other.baseMaterial),
    buffer(other.buffer),
    properties(other.properties),
    samplers(other.samplers) {}

MaterialInstance& MaterialInstance::operator=(const MaterialInstance& other) {
    if (this != &other) {
        name = other.name;
        baseMaterial = other.baseMaterial;
        buffer = other.buffer;
        properties = other.properties;
        samplers = other.samplers;
    }
    return *this;
}

MaterialInstance::MaterialInstance(MaterialInstance&& other) noexcept
  : name(std::move(other.name)),
    baseMaterial(other.baseMaterial),
    buffer(other.buffer),
    properties(std::move(other.properties)),
    samplers(std::move(other.samplers)) {
    other.baseMaterial = nullptr;
    other.buffer = nullptr;
}

MaterialInstance& MaterialInstance::operator=(
    MaterialInstance&& other
) noexcept {
    if (this != &other) {
        name = std::move(other.name);
        baseMaterial = other.baseMaterial;
        buffer = other.buffer;
        properties = std::move(other.properties);
        samplers = std::move(other.samplers);

        other.baseMaterial = nullptr;
        other.buffer = nullptr;
    }
    return *this;
}

void MaterialInstance::throwIfNoSampler(const std::string& samplerName) const {
    if (!baseMaterial->hasSampler(samplerName)) {
        throw std::invalid_argument(
            "Material \"" + name + "\" has no sampler \"" + samplerName + "\""
        );
    }
}

void MaterialInstance::throwIfNoProperty(
    const std::string& propertyName
) const {
    if (!baseMaterial->hasProperty(propertyName)) {
        throw std::invalid_argument(
            "Material \"" + name + "\" has no property \"" + propertyName + 
            "\""
        );
    }
}

bool MaterialInstance::hasProperty(const std::string& name) const {
    return baseMaterial->hasProperty(name);
}

bool MaterialInstance::hasSampler(const std::string& name) const {
    return baseMaterial->hasSampler(name);
}

void MaterialInstance::setSampler(
    const std::string& _name,
    std::shared_ptr<Texture> _texture
) {
    throwIfNoSampler(_name);
    samplers[_name] = _texture;
    
    uint64_t handle = _texture ? _texture->getHandle() : 0ULL;
    uint32_t lowerBits = static_cast<uint32_t>(handle);
    uint32_t upperBits = static_cast<uint32_t>(handle >> 32);   
    properties.setProperty<glm::uvec2>(_name, glm::uvec2(lowerBits, upperBits));
}

std::shared_ptr<Texture> MaterialInstance::getSampler(
    const std::string& samplerName
) const {
    throwIfNoSampler(samplerName);
    auto it = samplers.find(samplerName);
    if (it != samplers.end()) {
        return it->second;
    }
    return nullptr;
}

void MaterialInstance::bindSamplers() const {
    const GLuint GL_NO_BIND = 0;

    for (const auto& samplerDef : baseMaterial->getSamplerDefinitions()) {
        auto it = samplers.find(samplerDef.name);

        if (it != samplers.end() && it->second) {
            glBindTextureUnit(samplerDef.slot, it->second->getId());
        } else {
            glBindTextureUnit(samplerDef.slot, GL_NO_BIND);
        }
    }
}

void MaterialInstance::unbindSamplers() const {
    const GLuint GL_NO_BIND = 0;
    for (const auto& samplerDef : baseMaterial->getSamplerDefinitions()) {
        glBindTextureUnit(samplerDef.slot, GL_NO_BIND);
    }
}
