#ifndef ENGINE_MATERIALS_MATERIAL_H_
#define ENGINE_MATERIALS_MATERIAL_H_

#include <string>
#include <memory>

#include "MaterialLayout.hpp"
#include "MaterialGraphicsConfig.hpp"
#include "PropertyDataStorage.hpp"
#include "../graphics/SamplerDefinition.hpp"
#include "MaterialDescriptor.hpp"

class Shader;
class Texture;

class Material {
    friend class MaterialBuilder;

    MaterialDescriptor descriptor;

    std::shared_ptr<Shader> shader;
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

    std::string getName() const;
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
    const T& getPropertyDefaultValue(const std::string& name);

    void bindShader(std::shared_ptr<Shader> _shader) { shader = _shader; }
    std::shared_ptr<Shader> getShader() const { return shader; }
};

template <typename T>
const T& Material::getPropertyDefaultValue(const std::string& name) {
    return defaultValues.getProperty<T>(name); 
}

#endif // ENGINE_MATERIALS_MATERIAL_H_
