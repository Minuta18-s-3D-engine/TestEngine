#ifndef ENGINE_MATERIALS_MATERIALBUILDER_H_
#define ENGINE_MATERIALS_MATERIALBUILDER_H_

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <memory>

#include "Material.hpp"
#include "MaterialGraphicsConfig.hpp"
#include "PropertyDataStorage.hpp"
#include "MaterialLayout.hpp"
#include "MaterialDataBuffer.hpp"
#include "../graphics/SamplerType.hpp"
#include "../graphics/SamplerDefinition.hpp"
#include "../assets/AssetManager.hpp"
#include "../graphics/Texture.hpp"

class Shader;

class MaterialBuilder {
    MaterialGraphicsConfig cfg;
    std::string name;

    using BinderFunc = std::function<void(PropertyDataStorage&)>;
    std::vector<BinderFunc> propertyBinders;
    std::unordered_map<std::string, size_t> samplersIndexes;
    std::vector<SamplerDefinition> samplerDefinitions;
    std::unordered_map<std::string, std::shared_ptr<Texture>> samplerDefaults;

    MaterialLayout resultLayout; 
    const AssetManager& assetManager;

    std::shared_ptr<Texture> missingTexture;
public:
    MaterialBuilder(
        const std::string& _name, 
        MaterialGraphicsConfig _cfg,
        const AssetManager& _assetManager
    );

    template <typename T>
    MaterialBuilder& addProperty(const std::string& name);
    
    template <typename T>
    MaterialBuilder& addProperty(
        const std::string& name, const T& defaultValue
    );

    MaterialBuilder& addSampler(const std::string& name);
    MaterialBuilder& addSampler(const std::string& name, SamplerType type);
    MaterialBuilder& addSampler(
        const std::string& name, SamplerType type, 
        std::shared_ptr<Texture> defaultTexture
    );

    Material finalize(MaterialDataBuffer& buffer);
};

template <typename T>
MaterialBuilder& MaterialBuilder::addProperty(const std::string& name) {
    resultLayout.addProperty<T>(name);

    return *this;
}

template <typename T>
MaterialBuilder& MaterialBuilder::addProperty(
    const std::string& name, const T& defaultValue
) {
    resultLayout.addProperty<T>(name);

    propertyBinders.push_back(
        [name, defaultValue](PropertyDataStorage& storage) {
            storage.setProperty<T>(name, defaultValue);
        }
    );

    return *this;
}

#endif // ENGINE_MATERIALS_MATERIALBUILDER_H_
