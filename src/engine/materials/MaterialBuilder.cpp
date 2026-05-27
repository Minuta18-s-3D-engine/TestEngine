#include "MaterialBuilder.hpp"

MaterialBuilder::MaterialBuilder(
    const std::string& _name, MaterialGraphicsConfig _cfg,
    const AssetManager& _assetManager
) : assetManager(_assetManager) {
    resultDescriptor.name = _name;
    resultDescriptor.config = _cfg;

    missingTexture = _assetManager.getShared<Texture>(_cfg.missingTextureKey);
    if (missingTexture == nullptr) {
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
    const std::string& name, SamplerType type, 
    std::shared_ptr<Texture> defaultTexture
) {
    if (resultDescriptor.samplerIndexes.contains(name)) {
        return *this;
    }

    if (!defaultTexture) {
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

    uint64_t handle = defaultTexture->getHandle();
    propertyBinders.push_back([handle, name](PropertyDataStorage& storage) {
        uint32_t lowerBits = static_cast<uint32_t>(handle);
        uint32_t upperBits = static_cast<uint32_t>(handle >> 32);   
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

    return Material(
        std::move(resultDescriptor),
        std::move(samplerDefaults),
        std::move(tempStorage)
    );
}
