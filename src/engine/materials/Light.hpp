#ifndef ENGINE_LIGHT_LIGHT_H_
#define ENGINE_LIGHT_LIGHT_H_

#include "glm/glm.hpp"
#include "../graphics/Shader.hpp"
#include "BaseMaterial.hpp"

class Light : public BaseMaterial {
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    glm::vec3 pos;

    Light(
        glm::vec3 pos,
        glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3 specular = glm::vec3(0.2f, 0.2f, 0.2f)
    );
    ~Light();

    void passToShader(Shader& shader);
};

#endif ENGINE_LIGHT_LIGHT_H_
