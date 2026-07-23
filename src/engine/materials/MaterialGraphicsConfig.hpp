#ifndef ENGINE_MATERIALS_MATERIALGRAPHICSCONFIG_H_
#define ENGINE_MATERIALS_MATERIALGRAPHICSCONFIG_H_

#include <string>
#include "engine/project/VirtualPath.hpp"

enum class RenderingType { Deferred, Forward };

struct MaterialGraphicsConfig {
    RenderingType renderingType = RenderingType::Deferred;
    VirtualPath missingTextureKey = "core://assets/textures/missing.png";
};

#endif // ENGINE_MATERIALS_MATERIALGRAPHICSCONFIG_H_
