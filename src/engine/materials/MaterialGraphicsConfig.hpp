#ifndef ENGINE_MATERIALS_MATERIALGRAPHICSCONFIG_H_
#define ENGINE_MATERIALS_MATERIALGRAPHICSCONFIG_H_

enum class RenderingType { Deferred, Forward };

struct MaterialGraphicsConfig {
    RenderingType renderingType = RenderingType::Deferred;
};

#endif // ENGINE_MATERIALS_MATERIALGRAPHICSCONFIG_H_
