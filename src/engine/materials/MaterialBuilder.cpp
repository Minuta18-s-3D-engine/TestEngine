#include "MaterialBuilder.hpp"

MaterialBuilder::MaterialBuilder(
    const std::string& _name, MaterialGraphicsConfig _cfg
) : name(_name), cfg(_cfg) {}

MaterialBuilder& MaterialBuilder::addSampler(const std::string& name) {
    return addSampler(name, SamplerType::Texture2D);
}

MaterialBuilder& MaterialBuilder::addSampler(
    const std::string& name, SamplerType type
) {
    if (samplersIndexes.contains(name)) {
        return *this;
    }

    SamplerDefinition def;
    def.name = name;
    def.type = type;
    def.slot = static_cast<uint32_t>(samplersIndexes.size());
    samplersIndexes[name] = samplersIndexes.size();
    samplerDefinitions.push_back(def);

    // NOTE: It was initially planned to use uint64_t, as bindless_textures 
    // docs suggest. Unfortunately, this causes mesa driver bug, which leads
    // to segmentation fault.
    resultLayout.addProperty<glm::uvec2>(name);

    propertyBinders.push_back([name](PropertyDataStorage& storage) {
        storage.setProperty<glm::uvec2>(name, {0U, 0U});
    });

    return *this;
}

Material MaterialBuilder::finalize(MaterialDataBuffer& buffer) {
    resultLayout.finalize();

    PropertyDataStorage tempStorage(resultLayout, buffer);

    for (const auto& binder : propertyBinders) {
        binder(tempStorage);
    }

    return Material(
        name,
        cfg,
        std::move(resultLayout),
        std::move(tempStorage),
        std::move(samplersIndexes),
        std::move(samplerDefinitions)
    );
}
