#include "MaterialBuilder.hpp"

MaterialBuilder::MaterialBuilder(
    const std::string& _name, MaterialGraphicsConfig _cfg
) : name(_name), cfg(_cfg) {}

MaterialBuilder& MaterialBuilder::addSampler(const std::string& name) {
    samplers.push_back(name);

    return *this;
}

Material MaterialBuilder::finalize() {
    resultLayout.finalize();

    // TODO
}
