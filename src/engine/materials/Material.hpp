#ifndef ENGINE_MATERIALS_MATERIAL_H_
#define ENGINE_MATERIALS_MATERIAL_H_

#include <string>

#include "MaterialLayout.hpp"
#include "MaterialGraphicsConfig.hpp"
#include "PropertyDataStorage.hpp"
#include "../graphics/SamplerDefinition.hpp"
#include "MaterialDescriptor.hpp"
#include "engine/resource/ResourceHandle.hpp"

class Shader;

class Material {
    friend class MaterialBuilder;

    MaterialDescriptor descriptor;

    ResourceHandle<Shader> shaderHandle;
    PropertyDataStorage defaultValues;
    MaterialDescriptor::SamplerMap samplerDefaults;

    Material(
        MaterialDescriptor&& _descriptor,
        MaterialDescriptor::SamplerMap&& _samplerDefaults,
        PropertyDataStorage&& _storage
    );
public:
    Material(const Material& other) = delete;
    Material& operator=(const Material& other) = delete;

    Material(Material&& other) noexcept;
    Material& operator=(Material&& other) noexcept;

    const std::string& getName() const;
    const MaterialGraphicsConfig& getConfig() const;
    const MaterialLayout& getLayout() const;
    const PropertyDataStorage& getDefaultValues() const;
    const std::vector<SamplerDefinition>& getSamplerDefinitions() const;
    const MaterialDescriptor::SamplerMap& getSamplerDefaults() const;
    const MaterialDescriptor& getDescriptor() const;

    bool hasProperty(const std::string& name) const;
    bool hasDefaultValue(const std::string& name) const;
    MaterialLayout::PropertyType getPropertyType(
        const std::string& name
    ) const;

    bool hasSampler(const std::string& name) const;
    const SamplerDefinition& getSampler(const std::string& name) const;

    template <typename T>
    T getPropertyDefaultValue(const std::string& name);

    void bindShader(ResourceHandle<Shader> _shader) { shaderHandle = _shader; }
    ResourceHandle<Shader> getShader() const { return shaderHandle; }
};

template <typename T>
T Material::getPropertyDefaultValue(const std::string& name) {
    return defaultValues.getProperty<T>(name); 
}

#endif // ENGINE_MATERIALS_MATERIAL_H_
