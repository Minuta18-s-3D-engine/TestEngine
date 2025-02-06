#ifndef ENGINE_MATERIAL_BASEMATERIAL_H_
#define ENGINE_MATERIAL_BASEMATERIAL_H_

#include "../graphics/Shader.hpp"

class BaseMaterial {
public:
    BaseMaterial();
    virtual ~BaseMaterial();

    virtual void passToShader(Shader& shader) = 0;
};

#endif // ENGINE_MATERIAL_BASEMATERIAL_H_
