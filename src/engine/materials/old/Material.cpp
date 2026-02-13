#include "Material.hpp"

Material::Material(
    glm::vec3 color,
    glm::vec3 ambient,
    glm::vec3 diffuse,
    glm::vec3 specular,
    float shininess
) : ambient(ambient), diffuse(diffuse), specular(specular), color(color),
    shininess(shininess) {

}

Material::~Material() {

}

void Material::passToShader(Shader& shader, std::string name) {
    shader.setUniform3f(name + ".ambient", ambient);
    shader.setUniform3f(name + ".diffuse", diffuse);
    shader.setUniform3f(name + ".specular", specular);
    shader.setUniform3f(name + ".color", color);
    shader.setUniform1f(name + ".shininess", shininess);
}

