#ifndef ENGINE_MATERIALS_MATERIALINSTANCE_H_
#define ENGINE_MATERIALS_MATERIALINSTANCE_H_

#include <string>
#include <unordered_map>

#include "Material.hpp"
#include "MaterialDataBuffer.hpp"
#include "../graphics/Texture.hpp"

class MaterialInstance {
    std::string name;

    const Material* baseMaterial;
    MaterialDataBuffer* buffer;
    PropertyDataStorage properties;

    std::unordered_map<std::string, std::shared_ptr<Texture>> samplers;

    void throwIfNoSampler(const std::string& samplerName) const;
    void throwIfNoProperty(const std::string& propertyName) const;
public:
    MaterialInstance(
        const std::string& _name, 
        const Material& _material, 
        MaterialDataBuffer& _buffer
    );

    ~MaterialInstance() = default;
    
    MaterialInstance(const MaterialInstance& other);
    MaterialInstance& operator=(const MaterialInstance& other);

    MaterialInstance(MaterialInstance&& other) noexcept;
    MaterialInstance& operator=(MaterialInstance&& other) noexcept;

    bool hasProperty(const std::string& name) const;

    template <typename T>
    void setProperty(const std::string& name, const T& value);

    template <typename T>
    const T& getProperties(const std::string& name) const;

    bool hasSampler(const std::string& name) const;
    void setSampler(const std::string& name, std::shared_ptr<Texture> texture);
    std::shared_ptr<Texture> getSampler(const std::string& name) const;

    void bindSamplers() const;
    void unbindSamplers() const;

    const Material& getMaterial() const { return *baseMaterial; }
    const PropertyDataStorage& getProperties() const { return properties; }
};

template <typename T>
void MaterialInstance::setProperty(const std::string& name, const T& value) {
    throwIfNoProperty(name);
    properties.setProperty<T>(name, value);
}

template <typename T>
const T& MaterialInstance::getProperties(const std::string& name) const {
    throwIfNoProperty(name);
    return properties.getProperty<T>(name);
}

#endif // ENGINE_MATERIALS_MATERIALINSTANCE_H_
