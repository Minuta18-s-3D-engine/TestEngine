#ifndef ENGINE_MATERIALS_MATERIAL_H_
#define ENGINE_MATERIALS_MATERIAL_H_

#include <string>

#include "MaterialLayout.hpp"
#include "MaterialGraphicsConfig.hpp"
#include "PropertyDataStorage.hpp"

class Material {
    friend class MaterialBuilder;

    std::string name;
    MaterialGraphicsConfig cfg;
    MaterialLayout layout;
    PropertyDataStorage defaultValues;

    Material(
        const std::string& _name,
        MaterialGraphicsConfig _cfg,
        MaterialLayout _layout,
        PropertyDataStorage&& _storage
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

    bool hasProperty(const std::string& name);
    bool hasDefaultValue(const std::string& name);
    MaterialLayout::PropertyType getPropertyType(const std::string& name);

    template <typename T>
    const T& getPropertyDefaultValue(const std::string& name);
};

template <typename T>
const T& Material::getPropertyDefaultValue(const std::string& name) {
    return defaultValues.getProperty<T>(name); 
}

#endif // ENGINE_MATERIALS_MATERIAL_H_
