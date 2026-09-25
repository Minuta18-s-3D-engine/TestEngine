#include "MaterialInstance.hpp"

#include "engine/graphics/Shader.hpp"
#include "engine/graphics/Texture.hpp"

MaterialInstance::MaterialInstance(
    const Material& baseMaterial_,
    MaterialDataBuffer& buffer_,
    ResourceManager& resourceManager_
) : buffer(&buffer_),
    resourceManager(&resourceManager_),
    baseMaterial(&baseMaterial_),
    properties(baseMaterial_.getProperties(), buffer_),
    samplers(baseMaterial_.getSamplers())
{
    const auto& shader = resourceManager_.require<Shader>(
        baseMaterial_.getShader());
    properties.bindLayout(shader.getLayout());
}

MaterialInstance::MaterialInstance(MaterialInstance&& other) noexcept
  : buffer(other.buffer),
    resourceManager(other.resourceManager),
    baseMaterial(other.baseMaterial),
    properties(std::move(other.properties)),
    samplers(std::move(other.samplers))
{
    other.baseMaterial = nullptr;
    other.buffer = nullptr;
}

MaterialInstance& MaterialInstance::operator=(
    MaterialInstance&& other
) noexcept {
    if (this != &other) {
        baseMaterial = other.baseMaterial;
        buffer = other.buffer;
        resourceManager = other.resourceManager;
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
            "Material  has no sampler \"" + samplerName
        );
    }
}

void MaterialInstance::throwIfNoProperty(
    const std::string& propertyName
) const {
    if (!baseMaterial->hasProperty(propertyName)) {
        throw std::invalid_argument(
            "Material  has no property \"" + propertyName
        );
    }
}

bool MaterialInstance::hasProperty(const std::string& propertyName) const {
    return baseMaterial->hasProperty(propertyName);
}

bool MaterialInstance::hasSampler(const std::string& propertyName) const {
    return baseMaterial->hasSampler(propertyName);
}

void MaterialInstance::setSampler(
    const std::string& _name,
    ResourceHandle<Texture> _texture
) {
    throwIfNoSampler(_name);
    if (!resourceManager->exists(_texture)) {
        return;
    }

    const Texture& textureResource = resourceManager->require(_texture);
    samplers[_name] = _texture;

    const uint64_t handle = textureResource.getHandle();
    const auto lowerBits = static_cast<uint32_t>(handle);
    const auto upperBits = static_cast<uint32_t>(handle >> 32);
    properties.setProperty<glm::uvec2>(_name, glm::uvec2(lowerBits, upperBits));
}

ResourceHandle<Texture> MaterialInstance::getSampler(
    const std::string& samplerName
) const {
    throwIfNoSampler(samplerName);
    auto it = samplers.find(samplerName);
    if (it != samplers.end()) {
        return it->second;
    }
    return ResourceHandle<Texture>::createNullHandle();
}

void MaterialInstance::bindSamplers(uint32_t startSlot) const {
    const GLuint GL_NO_BIND = 0;

    const auto& shader = resourceManager->require(baseMaterial->getShader());
    const auto& shaderLayout = shader.getLayout();

    for (const auto& samplerDef : shaderLayout.getSamplers()) {
        const auto it = samplers.find(samplerDef.name);

        if (it != samplers.end() && resourceManager->exists(it->second)) {
            Texture& texture = resourceManager->require(it->second);
            glBindTextureUnit(
                samplerDef.binding + startSlot, texture.getId()
            );
        } else {
            glBindTextureUnit(samplerDef.binding, GL_NO_BIND);
        }
    }
}

void MaterialInstance::unbindSamplers() const {
    const auto& shader = resourceManager->require(baseMaterial->getShader());
    const auto& shaderLayout = shader.getLayout();

    for (const auto& samplerDef : shaderLayout.getSamplers()) {
        constexpr GLuint GL_NO_BIND = 0;
        glBindTextureUnit(samplerDef.binding, GL_NO_BIND);
    }
}
