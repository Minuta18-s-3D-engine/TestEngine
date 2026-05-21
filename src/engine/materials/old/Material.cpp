#include "Material.hpp"

OldMaterial::OldMaterial(
    glm::vec3 color,
    glm::vec3 ambient,
    glm::vec3 diffuse,
    glm::vec3 specular,
    float shininess
) : ambient(ambient), diffuse(diffuse), specular(specular), color(color),
    shininess(shininess) {

}

OldMaterial::~OldMaterial() {

}

void OldMaterial::passToShader(Shader& shader, std::string name) {
    shader.setUniform(name + ".ambient", ambient);
    shader.setUniform(name + ".diffuse", diffuse);
    shader.setUniform(name + ".specular", specular);
    shader.setUniform(name + ".color", color);
    shader.setUniform(name + ".shininess", shininess);
}

