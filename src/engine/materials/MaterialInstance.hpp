#ifndef ENGINE_MATERIALS_MATERIALINSTANCE_H_
#define ENGINE_MATERIALS_MATERIALINSTANCE_H_

#include <memory>

#include "TypedPropertyStorage.hpp"
#include "Material.hpp"

class Shader;

class MaterialInstance {
    std::shared_ptr<Shader> shader;
};

#endif // ENGINE_MATERIALS_MATERIALINSTANCE_H_
