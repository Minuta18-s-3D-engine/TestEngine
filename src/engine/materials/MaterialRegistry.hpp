#ifndef ENGINE_MATERIALS_MATERIALINSTANCE_H_
#define ENGINE_MATERIALS_MATERIALINSTANCE_H_

#include "Material.hpp"
#include "../graphics/ShaderStorageBuffer.hpp"
class MaterialInstance;
#include "MaterialInstance.hpp"

class MaterialRegistry {
public:
    using MaterialID = size_t;
    using MaterialInstanceID = size_t;
public:
    MaterialRegistry() = default;

    MaterialID registerMaterial(const Material& material);
    MaterialInstanceID registerMaterialInstance(
        const MaterialInstance& instance
    );
};

#endif // ENGINE_MATERIALS_MATERIALINSTANCE_H_
