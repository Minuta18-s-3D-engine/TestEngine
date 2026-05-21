#include "MaterialBuilder.hpp"

MaterialBuilder::MaterialBuilder(
    const std::string& _name, MaterialGraphicsConfig _cfg, 
    std::shared_ptr<Shader> _shader
) : name(_name), cfg(_cfg), shader(_shader) {}

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

    resultLayout.addProperty<uint64_t>(name);

    propertyBinders.push_back([name](PropertyDataStorage& storage) {
        storage.setProperty<uint64_t>(name, 0ULL);
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
        shader,
        cfg,
        std::move(resultLayout),
        std::move(tempStorage),
        std::move(samplersIndexes),
        std::move(samplerDefinitions)
    );
}
