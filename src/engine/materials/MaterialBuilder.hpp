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
#include "../graphics/Texture.hpp"
#include "MaterialDescriptor.hpp"
#include "engine/resource/ResourceManager.hpp"
#include "engine/resource/ResourceHandle.hpp"

class Shader;

class MaterialBuilder {
    MaterialDescriptor resultDescriptor;

    MaterialDescriptor::SamplerMap samplerDefaults;

    ResourceManager* resourceManager;
    ResourceHandle<Texture> missingTexture;

    using BinderFunc = std::function<void(PropertyDataStorage&)>;
    std::vector<BinderFunc> propertyBinders;
public:
    MaterialBuilder(
        const std::string& _name, 
        MaterialGraphicsConfig _cfg,
        ResourceManager& _resourceManager
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
        ResourceHandle<Texture> defaultTexture
    );

    Material finalize(MaterialDataBuffer& buffer);
};

template <typename T>
MaterialBuilder& MaterialBuilder::addProperty(const std::string& name) {
    resultDescriptor.layout.addProperty<T>(name);

    return *this;
}

template <typename T>
MaterialBuilder& MaterialBuilder::addProperty(
    const std::string& name, const T& defaultValue
) {
    resultDescriptor.layout.addProperty<T>(name);

    propertyBinders.push_back(
        [name, defaultValue](PropertyDataStorage& storage) {
            storage.setProperty<T>(name, defaultValue);
        }
    );

    return *this;
}

#endif // ENGINE_MATERIALS_MATERIALBUILDER_H_
