#include "Light.hpp"

Light::Light(
    glm::vec3 pos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular
) : pos(pos), ambient(ambient), diffuse(diffuse), specular(specular) {

}

Light::~Light() = default;

void Light::passToShader(Shader& shader) {
    shader.setUniform3f("material.ambient", ambient);
    shader.setUniform3f("material.diffuse", diffuse);
    shader.setUniform3f("material.specular", specular);
    shader.setUniform3f("material.pos", pos);
}

