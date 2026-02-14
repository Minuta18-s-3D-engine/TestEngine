#ifndef ENGINE_MATERIALS_MATERIAL_H_
#define ENGINE_MATERIALS_MATERIAL_H_

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

#include "MaterialProperty.hpp"

class Material {
public:
    struct Property {
        bool hasDefaultProperty;
        std::unique_ptr<IMaterialProperty> defaultValue;
    };

    using PropertyMap = std::unordered_map<
        std::string, 
        Property
    >;
private:
    PropertyMap properties;
public:
    Material() = default;

    template<typename T>
    Material& addProperty(const std::string& name, const T& defaultValue) {
        properties[name] = {
            true,
            std::make_unique<MaterialProperty<T>>(defaultValue)
        };

        return *this;
    }

    template<typename T>
    Material& addProperty(const std::string& name) {
        properties[name] = {false, nullptr};

        return *this;
    }

    PropertyMap& getPropertyMap();
};

#endif // ENGINE_MATERIALS_MATERIAL_H_
