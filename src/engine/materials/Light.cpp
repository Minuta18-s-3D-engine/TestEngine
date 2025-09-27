#include "Light.hpp"

Light::Light(
    glm::vec3 pos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular
) : pos(pos), ambient(ambient), diffuse(diffuse), specular(specular) {

}

Light::Light(glm::vec3 pos, glm::vec3 color) : pos(pos), ambient(color), 
    diffuse(color), specular(color) {
    
}

Light::~Light() = default;

void Light::passToShader(Shader& shader, std::string name) {
    shader.setUniform3f(name + ".ambient", ambient);
    shader.setUniform3f(name + ".diffuse", diffuse);
    shader.setUniform3f(name + ".specular", specular);
    shader.setUniform3f(name + ".pos", pos);
}

