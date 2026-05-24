#ifndef ENGINE_MATERIALS_MATERIAL_H_
#define ENGINE_MATERIALS_MATERIAL_H_

#include <string>
#include <memory>

#include "MaterialLayout.hpp"
#include "MaterialGraphicsConfig.hpp"
#include "PropertyDataStorage.hpp"
#include "../graphics/SamplerDefinition.hpp"

class Shader;

class Material {
    friend class MaterialBuilder;

    std::string name;
    std::shared_ptr<Shader> shader;
    MaterialGraphicsConfig cfg;
    MaterialLayout layout;
    PropertyDataStorage defaultValues;

    std::unordered_map<std::string, size_t> samplersIndexes;
    std::vector<SamplerDefinition> samplerDefinitions;

    Material(
        const std::string& _name,
        MaterialGraphicsConfig _cfg,
        MaterialLayout _layout,
        PropertyDataStorage&& _storage,
        std::unordered_map<std::string, size_t>&& _samplerIndexes,
        std::vector<SamplerDefinition>&& samplerDefinitions
    );
public:
    Material(const Material& other) = delete;
    Material& operator=(const Material& other) = delete;

    Material(Material&& other) noexcept;
    Material& operator=(Material&& other) noexcept;

    std::string getName() const { return name; };
    const MaterialGraphicsConfig& getConfig() const { return cfg; }
    const MaterialLayout& getLayout() const { return layout; }
    const PropertyDataStorage& getDefaultValues() const { return defaultValues; }
    const std::vector<SamplerDefinition>& getSamplerDefinitions() const {
        return samplerDefinitions;
    }

    bool hasProperty(const std::string& name) const;
    bool hasDefaultValue(const std::string& name) const;
    MaterialLayout::PropertyType getPropertyType(const std::string& name) const;

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
