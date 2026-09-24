#ifndef ENGINE_MATERIALS_MATERIALBUILDER_H_
#define ENGINE_MATERIALS_MATERIALBUILDER_H_

#include <string>
#include <vector>
#include <functional>

#include "Material.hpp"
#include "PropertyDataStorage.hpp"
#include "engine/resource/ResourceManager.hpp"
#include "engine/resource/ResourceHandle.hpp"

class Shader;
class ShaderLayout;
class Texture;
class Material;
class MaterialGraphicsConfig;
class MaterialDataBuffer;

class MaterialBuilder {
    MaterialGraphicsConfig* graphicsConfig;
    ResourceManager* resourceManager;
    ResourceHandle<Texture> missingTexture;

    MaterialDataBuffer* buffer;

    using BinderFunc = std::function<void(PropertyDataStorage&)>;
    std::vector<BinderFunc> propertyBinders;

    ResourceHandle<Shader> materialShader;
    Material::SamplerDefaults defaultSamplers;
public:
    MaterialBuilder(
        ResourceHandle<Shader> materialShader_,
        ResourceManager& resourceManager_,
        MaterialGraphicsConfig& graphicsConfig_,
        MaterialDataBuffer& materialDataBuffer_
    );

    template <typename T>
    MaterialBuilder& setProperty(const std::string& name, const T& value);

    MaterialBuilder& setSampler(
        const std::string& name, ResourceHandle<Texture> texture
    );

    Material finalize();
};

template<typename T>
MaterialBuilder& MaterialBuilder::setProperty(const std::string &name, const T &value) {
    propertyBinders.emplace_back([name, value] (PropertyDataStorage& s) {
        s.setProperty<T>(name, value);
    });

    return *this;
}

#endif // ENGINE_MATERIALS_MATERIALBUILDER_H_
