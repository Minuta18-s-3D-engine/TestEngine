#include "Light.hpp"

Light::Light(
    glm::vec3 _position,
    glm::vec3 _color,
    float _linear,
    float _quadratic,
    float _radius
) : position(_position), color(_color), linear(_linear), 
    quadratic(_quadratic), radius(_radius) {}

Light::Light(
    glm::vec3 _position,
    glm::vec3 _color 
) : position(_position), color(_color), linear(1.0f), 
    quadratic(1.0f), radius(10.0f) {}

Light::~Light() = default;

void Light::passToShader(Shader& shader, std::string name) {
    shader.setUniform3f(name + ".position", position);
    shader.setUniform3f(name + ".color", color);
    shader.setUniform1f(name + ".linear", linear);
    shader.setUniform1f(name + ".quadratic", quadratic);
    shader.setUniform1f(name + ".radius", radius);
}

void Light::passToShader(Shader& shader, std::string name, uint index) {
    std::string stringIndex = std::to_string(index);
    shader.setUniform3f(name + "[" + stringIndex + "]" + ".position", position);
    shader.setUniform3f(name + "[" + stringIndex + "]" + ".color", color);
    shader.setUniform1f(name + "[" + stringIndex + "]" + ".linear", linear);
    shader.setUniform1f(name + "[" + stringIndex + "]" + ".quadratic", quadratic);
    shader.setUniform1f(name + "[" + stringIndex + "]" + ".radius", radius);
}
