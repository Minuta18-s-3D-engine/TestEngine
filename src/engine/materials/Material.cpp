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

void Material::passToShader(Shader& shader) {
    shader.use();
    shader.setUniform3f("material.ambient", ambient);
    shader.setUniform3f("material.diffuse", diffuse);
    shader.setUniform3f("material.specular", specular);
    shader.setUniform3f("material.color", color);
    shader.setUniform1f("material.shininess", shininess);
}

