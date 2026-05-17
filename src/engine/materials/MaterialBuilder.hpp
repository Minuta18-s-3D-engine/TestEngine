#ifndef ENGINE_MATERIALS_MATERIALBUILDER_H_
#define ENGINE_MATERIALS_MATERIALBUILDER_H_

#include <string>
#include <vector>
#include <functional>

#include "Material.hpp"
#include "MaterialGraphicsConfig.hpp"
#include "PropertyDataStorage.hpp"
#include "MaterialLayout.hpp"

class MaterialBuilder {
    MaterialGraphicsConfig cfg;
    std::string name;

    using BinderFunc = std::function<void(PropertyDataStorage&)>;
    std::vector<BinderFunc> propertyBinders;
    std::vector<std::string> samplers;

    MaterialLayout resultLayout; 
public:
    MaterialBuilder(const std::string& _name, MaterialGraphicsConfig _cfg);

    template <typename T>
    MaterialBuilder& addProperty(const std::string& name);
    
    template <typename T>
    MaterialBuilder& addProperty(
        const std::string& name, const T& defaultValue
    );

    MaterialBuilder& addSampler(const std::string& name);

    Material finalize();
};

template <typename T>
MaterialBuilder& MaterialBuilder::addProperty(const std::string& name) {
    resultLayout.addProperty(name);

    propertyBinders.push_back(
        [name, defaultValue](PropertyDataStorage& storage) {
            storage.setProperty<T>(name);
        }
    );

    return *this;
}

template <typename T>
MaterialBuilder& MaterialBuilder::addProperty(
    const std::string& name, const T& defaultValue
) {
    resultLayout.addProperty(name);

    propertyBinders.push_back(
        [name, defaultValue](PropertyDataStorage& storage) {
            storage.setProperty<T>(name, defaultValue);
        }
    );

    return *this;
}

#endif // ENGINE_MATERIALS_MATERIALBUILDER_H_
