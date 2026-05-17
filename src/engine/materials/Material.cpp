#include "Material.hpp"

Material::Material(
    const std::string& _name,
    MaterialGraphicsConfig _cfg,
    MaterialLayout _layout,
    PropertyDataStorage&& _storage
) : name(_name), cfg(_cfg), layout(std::move(_layout)),
    defaultValues(std::move(_storage)) {}

Material::Material(Material&& other) noexcept
  : name(std::move(other.name)), 
    cfg(std::move(other.cfg)),
    layout(std::move(other.layout)),
    defaultValues(std::move(other.defaultValues)) {}

Material& Material::operator=(Material&& other) noexcept {
    if (this != &other) {
        name = std::move(other.name);
        cfg = std::move(other.cfg);
        layout = std::move(other.layout);
        defaultValues = std::move(other.defaultValues);
    }
    return *this;
}

bool Material::hasProperty(const std::string& name) {
    return layout.hasProperty(name);
}

bool Material::hasDefaultValue(const std::string& name) {
    return hasProperty(name);
}

MaterialLayout::PropertyType Material::getPropertyType(
    const std::string& name
) {
    return layout.getPropertyInfo(name).type;
}
