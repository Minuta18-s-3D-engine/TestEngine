#ifndef ENGINE_MATERIAL_LIGHT_H_
#define ENGINE_MATERIAL_LIGHT_H_

#include "glm/glm.hpp"
#include "../graphics/Shader.hpp"
#include "BaseMaterial.hpp"
#include <string>

class Light : public BaseMaterial {
    float calcRadius();
public:
    glm::vec3 color;
    glm::vec3 position;

    float linear;
    float quadratic;

    Light(
        glm::vec3 _position,
        glm::vec3 _color,
        float _linear,
        float _quadratic
    );
    Light(
        glm::vec3 _position,
        glm::vec3 _color 
    );
    ~Light();

    void passToShader(Shader& shader, std::string name);
    void passToShader(Shader& shader, std::string name, uint index);

    static std::shared_ptr<Light> calcLight(
        glm::vec3 _position,
        glm::vec3 _color,
        float _linear,
        float _quadratic
    );
};

#endif //  ENGINE_MATERIAL_LIGHT_H_
