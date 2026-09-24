#ifndef ENGINE_MATERIALS_MATERIALINSTANCE_H_
#define ENGINE_MATERIALS_MATERIALINSTANCE_H_

#include <string>

#include "Material.hpp"
#include "engine/resource/ResourceManager.hpp"

class MaterialInstance {
    MaterialDataBuffer* buffer;
    ResourceManager* resourceManager;

    const Material* baseMaterial;
    PropertyDataStorage properties;
    Material::SamplerDefaults samplers;

    void throwIfNoSampler(const std::string& samplerName) const;
    void throwIfNoProperty(const std::string& propertyName) const;
public:
    MaterialInstance(
        const Material& baseMaterial_,
        MaterialDataBuffer& buffer_,
        ResourceManager& resourceManager_
    );
    ~MaterialInstance() = default;
    
    MaterialInstance(const MaterialInstance& other) = default;
    MaterialInstance& operator=(const MaterialInstance& other) = default;

    MaterialInstance(MaterialInstance&& other) noexcept;
    MaterialInstance& operator=(MaterialInstance&& other) noexcept;

    [[nodiscard]] bool hasProperty(const std::string& propertyName) const;
    template <typename T>
    [[nodiscard]] T getProperty(const std::string& propertyName) const;
    template <typename T>
    void setProperty(const std::string& propertyName, const T& value);

    [[nodiscard]] bool hasSampler(const std::string& samplerName) const;
    [[nodiscard]] ResourceHandle<Texture> getSampler(const std::string& samplerName) const;
    void setSampler(const std::string& samplerName, ResourceHandle<Texture> value);

    void bindSamplers(uint32_t startSlot = 0) const;
    void unbindSamplers() const;

    const Material& getMaterial() const { return *baseMaterial; }
    const PropertyDataStorage& getProperties() const { return properties; }
};

template <typename T>
void MaterialInstance::setProperty(const std::string& propertyName, const T& value) {
    throwIfNoProperty(propertyName);
    properties.setProperty<T>(propertyName, value);
}

template <typename T>
T MaterialInstance::getProperty(const std::string& propertyName) const {
    throwIfNoProperty(propertyName);
    return properties.getProperty<T>(propertyName);
}

#endif // ENGINE_MATERIALS_MATERIALINSTANCE_H_
