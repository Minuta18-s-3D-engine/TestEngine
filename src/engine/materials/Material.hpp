#ifndef ENGINE_MATERIAL_MATERIALS_H_
#define ENGINE_MATERIAL_MATERIALS_H_

#include "glm/glm.hpp"
#include "../graphics/Shader.hpp"
#include "BaseMaterial.hpp"

class Material : public BaseMaterial {
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    glm::vec3 color;
    float shininess;

    Material(
        glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3 specular = glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3 color = glm::vec3(0.8f, 0.8f, 0.8f),
        float shininess = 8
    );
    ~Material();

    void passToShader(Shader& shader);
};

#endif ENGINE_MATERIAL_MATERIALS_H_
