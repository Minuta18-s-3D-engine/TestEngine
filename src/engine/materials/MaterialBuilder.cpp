#include "MaterialBuilder.hpp"

#include "engine/graphics/Texture.hpp"
#include "MaterialGraphicsConfig.hpp"
#include "engine/graphics/Shader.hpp"

MaterialBuilder::MaterialBuilder(
    ResourceHandle<Shader> materialShader_,
    ResourceManager& resourceManager_,
    MaterialGraphicsConfig& graphicsConfig_,
    MaterialDataBuffer& materialDataBuffer_
) : graphicsConfig(&graphicsConfig_),
    resourceManager(&resourceManager_),
    buffer(&materialDataBuffer_),
    materialShader(materialShader_) {

    missingTexture = resourceManager->load<Texture>(
        graphicsConfig->missingTextureKey
    );
    if (!missingTexture.isValid()) {
        throw std::invalid_argument("Invalid missingTexture key");
    }
}

MaterialBuilder &MaterialBuilder::setSampler(
    const std::string &name, ResourceHandle<Texture> texture
) {
    const auto& shader = resourceManager->require<Shader>(materialShader);

    if (!shader.getLayout().hasSampler(name)) {
        throw std::invalid_argument("Unknown sampler: " + name);
    }
    if (!texture.isValid()) texture = missingTexture;

    defaultSamplers[name] = texture;

    const auto& sampler = shader.getLayout().getSampler(name);
    if (sampler.handleOffset != ShaderLayout::NO_HANDLE) return *this;

    const Texture& t = resourceManager->require(texture);
    uint64_t handle = t.getHandle();
    propertyBinders.emplace_back([name, handle] (PropertyDataStorage& s) {
        s.setProperty<glm::uvec2>(name, {
            static_cast<uint32_t>(handle),
            static_cast<uint32_t>(handle >> 32)
        });
    });

    return *this;
}

Material MaterialBuilder::finalize() {
    const auto& shader = resourceManager->require<Shader>(materialShader);

    PropertyDataStorage tempStorage(shader.getLayout(), *buffer);

    for (const auto& binder : propertyBinders) {
        binder(tempStorage);
    }

    return {
        *graphicsConfig, materialShader,
        std::move(tempStorage), std::move(defaultSamplers)
    };
}
