#ifndef ENGINE_MATERIAL_LIGHT_H_
#define ENGINE_MATERIAL_LIGHT_H_

#include "glm/glm.hpp"
#include "../graphics/Shader.hpp"
#include "BaseMaterial.hpp"
#include <string>

class Light : public BaseMaterial {
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    glm::vec3 pos;
    float brightness;

    Light(glm::vec3 pos,
        glm::vec3 ambient,
        glm::vec3 diffuse,
        glm::vec3 specular,
        float brightness);
    Light(glm::vec3 pos,
        glm::vec3 color, 
        float brightness);
    ~Light();

    void passToShader(Shader& shader, std::string name);
};

#endif //  ENGINE_MATERIAL_LIGHT_H_
