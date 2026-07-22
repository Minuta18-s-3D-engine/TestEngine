#ifndef ENGINE_MATERIALS_MATERIALINSTANCE_H_
#define ENGINE_MATERIALS_MATERIALINSTANCE_H_

#include <string>
#include <unordered_map>

#include "Material.hpp"
#include "MaterialDataBuffer.hpp"
#include "MaterialDescriptor.hpp"
#include "engine/graphics/Texture.hpp"
#include "engine/resource/ResourceManager.hpp"

class MaterialInstance {
    std::string name;

    const Material* baseMaterial;
    const MaterialDescriptor* descriptor;
    MaterialDataBuffer* buffer;
    ResourceManager* resourceManager;

    PropertyDataStorage properties;

    MaterialDescriptor::SamplerMap samplers;

    void throwIfNoSampler(const std::string& samplerName) const;
    void throwIfNoProperty(const std::string& propertyName) const;
public:
    MaterialInstance(
        std::string _name,
        const Material& _material, 
        MaterialDataBuffer& _buffer,
        ResourceManager& _resourceManager
    );

    ~MaterialInstance() = default;
    
    MaterialInstance(const MaterialInstance& other) = default;
    MaterialInstance& operator=(const MaterialInstance& other) = default;

    MaterialInstance(MaterialInstance&& other) noexcept;
    MaterialInstance& operator=(MaterialInstance&& other) noexcept;

    bool hasProperty(const std::string& propertyName) const;

    template <typename T>
    void setProperty(const std::string& propertyName, const T& value);

    template <typename T>
    T getProperty(const std::string& propertyName) const;

    bool hasSampler(const std::string& samplerName) const;
    void setSampler(const std::string& samplerName, ResourceHandle<Texture> texture);
    ResourceHandle<Texture> getSampler(const std::string& samplerName) const;

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
