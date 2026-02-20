#ifndef ENGINE_MATERIALS_MATERIALINSTANCE_H_
#define ENGINE_MATERIALS_MATERIALINSTANCE_H_

#include <memory>

#include "TypedPropertyStorage.hpp"
#include "Material.hpp"

class Shader;

class MaterialInstance {
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Material> material;
    TypedPropertyStorage propertyOverrides;
    Material::TextureStorage textureOverrides;
    std::string name;
public:
    MaterialInstance(
        const std::string& _name,
        std::shared_ptr<Shader> _shader,
        std::shared_ptr<Material> _material
    );

    template <typename T>
    MaterialInstance& setProperty(
        const std::string& name, const T& value
    ) {
        if (!material->hasProperty(name)) {
            throw std::invalid_argument(
                "Property '" + name + "' does not exist in the material."
            );
        }
        propertyOverrides.setProperty<T>(name, value);
        return *this;
    }

    template <typename T>
    MaterialInstance& setProperty(const std::string& name) {
        propertyOverrides.setProperty<T>(name);
        return *this;
    }

    template <typename T>
    const T& getProperty(const std::string& name) const {
        if (propertyOverrides.hasProperty(name)) {
            return propertyOverrides.getProperty<T>(name);
        }
        return material->getProperty<T>(name);
    }

    bool hasProperty(const std::string& name) const;

    const std::string& getName() const { return name; }
    std::shared_ptr<Shader> getShader() const { return shader; }
    std::shared_ptr<Material> getMaterial() const { return material; }

    const TypedPropertyStorage& getPropertyStorage() const;
    const Material::TextureStorage& getTextureStorage() const;
};

#endif // ENGINE_MATERIALS_MATERIALINSTANCE_H_
