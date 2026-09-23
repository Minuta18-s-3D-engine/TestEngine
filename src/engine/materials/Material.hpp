#ifndef ENGINE_MATERIALS_MATERIAL_H_
#define ENGINE_MATERIALS_MATERIAL_H_

#include <string>

#include "MaterialGraphicsConfig.hpp"
#include "PropertyDataStorage.hpp"
#include "engine/resource/ResourceHandle.hpp"

class Shader;
class Texture;

class Material {
public:
    using SamplerDefaults = std::unordered_map<std::string, ResourceHandle<Texture>>;
private:
    MaterialGraphicsConfig config;
    ResourceHandle<Shader> shader;

    PropertyDataStorage properties;
    SamplerDefaults samplers;
public:
    Material(
        MaterialGraphicsConfig config_,
        ResourceHandle<Shader> shader_,
        PropertyDataStorage&& defaultValues_,
        SamplerDefaults&& defaultSamplers_
    );

    Material(const Material& other) = delete;
    Material& operator=(const Material& other) = delete;

    Material(Material&& other) noexcept;
    Material& operator=(Material&& other) noexcept;

    const MaterialGraphicsConfig& getConfig() const { return config; };
    ResourceHandle<Shader> getShader() const { return shader; }
    const PropertyDataStorage& getProperties() const { return properties; }
    const SamplerDefaults& getSamplers() const { return samplers; }

    bool hasProperty(const std::string& propertyName) const;
    template <typename T>
    T getProperty(const std::string& propertyName) const;
    template <typename T>
    void setProperty(const std::string& propertyName, const T& value);

    bool hasSampler(const std::string& samplerName) const;
    ResourceHandle<Texture> getSampler(const std::string& samplerName) const;
    void setSampler(const std::string& samplerName, ResourceHandle<Texture> value);
};

#endif // ENGINE_MATERIALS_MATERIAL_H_
