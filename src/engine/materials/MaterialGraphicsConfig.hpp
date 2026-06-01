#ifndef ENGINE_MATERIALS_MATERIALGRAPHICSCONFIG_H_
#define ENGINE_MATERIALS_MATERIALGRAPHICSCONFIG_H_

#include <string>

enum class RenderingType { Deferred, Forward };

struct MaterialGraphicsConfig {
    RenderingType renderingType = RenderingType::Deferred;
    std::string missingTextureKey = "textures/missing";
};

#endif // ENGINE_MATERIALS_MATERIALGRAPHICSCONFIG_H_
