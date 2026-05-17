#include "MaterialBuilder.hpp"

MaterialBuilder::MaterialBuilder(
    const std::string& _name, MaterialGraphicsConfig _cfg
) : name(_name), cfg(_cfg) {}

MaterialBuilder& MaterialBuilder::addSampler(const std::string& name) {
    samplers.push_back(name);

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
        resultLayout,
        tempStorage
    );
}
