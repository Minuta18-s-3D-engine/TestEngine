#include "MaterialBuilder.hpp"

MaterialBuilder::MaterialBuilder(
    const std::string& _name, MaterialGraphicsConfig _cfg,
    ResourceManager& _resourceManager
) : resourceManager(&_resourceManager) {
    resultDescriptor.name = _name;
    resultDescriptor.config = _cfg;

    missingTexture = resourceManager->load<Texture>(_cfg.missingTextureKey);
    if (!missingTexture.isValid()) {
        throw std::invalid_argument("Invalid missingTexture key");
    }
}

MaterialBuilder& MaterialBuilder::addSampler(const std::string& name) {
    return addSampler(name, SamplerType::Texture2D);
}

MaterialBuilder& MaterialBuilder::addSampler(
    const std::string& name, SamplerType type
) {
    return addSampler(name, type, missingTexture);
}

MaterialBuilder& MaterialBuilder::addSampler(
    const std::string& name, const SamplerType type,
    ResourceHandle<Texture> defaultTexture
) {
    if (resultDescriptor.samplerIndexes.contains(name)) {
        return *this;
    }

    if (!defaultTexture.isValid()) {
        return *this;
    }

    SamplerDefinition def;
    def.name = name;
    def.type = type;
    def.slot = static_cast<uint32_t>(resultDescriptor.samplerIndexes.size());
    resultDescriptor.samplerIndexes[name] = 
        resultDescriptor.samplerIndexes.size();
    resultDescriptor.samplerDefinitions.push_back(def);
    samplerDefaults[name] = defaultTexture;

    // NOTE: It was initially planned to use uint64_t, as bindless_textures 
    // docs suggest. Unfortunately, this causes mesa driver bug, which leads
    // to segmentation fault.
    resultDescriptor.layout.addProperty<glm::uvec2>(name);

    const Texture& texture = resourceManager->require(defaultTexture);
    uint64_t handle = texture.getHandle();
    propertyBinders.emplace_back([handle, name](PropertyDataStorage& storage) {
        auto lowerBits = static_cast<uint32_t>(handle);
        auto upperBits = static_cast<uint32_t>(handle >> 32);
        storage.setProperty<glm::uvec2>(name, {lowerBits, upperBits});
    });

    return *this;
}

Material MaterialBuilder::finalize(MaterialDataBuffer& buffer) {
    resultDescriptor.layout.finalize();

    PropertyDataStorage tempStorage(resultDescriptor.layout, buffer);

    for (const auto& binder : propertyBinders) {
        binder(tempStorage);
    }

    return {
        std::move(resultDescriptor),
        std::move(samplerDefaults),
        std::move(tempStorage)
    };
}
