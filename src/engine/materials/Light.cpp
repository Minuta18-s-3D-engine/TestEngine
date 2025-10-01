#include "Light.hpp"

Light::Light(
    glm::vec3 pos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, 
    float brightness
) : pos(pos), ambient(ambient), diffuse(diffuse), specular(specular), 
    brightness(brightness) {}

Light::Light(glm::vec3 pos, glm::vec3 color, float brightness) : pos(pos), 
    ambient(color), diffuse(color), specular(color), brightness(brightness) {}

Light::~Light() = default;

void Light::passToShader(Shader& shader, std::string name) {
    shader.setUniform3f(name + ".ambient", ambient);
    shader.setUniform3f(name + ".diffuse", diffuse);
    shader.setUniform3f(name + ".specular", specular);
    shader.setUniform3f(name + ".pos", pos);
    shader.setUniform1f(name + ".brightness", brightness);
}

