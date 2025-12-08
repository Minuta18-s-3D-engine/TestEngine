#include "TextureMaterial.hpp"

TextureMaterial::TextureMaterial() : isActive(false) {}

TextureMaterial::TextureMaterial(
    std::shared_ptr<Texture> tex
) : tex(tex), isActive(true), type(TextureType::DIFFUSE) {}

TextureMaterial::TextureMaterial(
    std::shared_ptr<Texture> tex,
    TextureType type
) : tex(tex), isActive(true), type(type) {}

TextureMaterial::~TextureMaterial() {}

void TextureMaterial::passToShader(Shader& shader, std::string name) {
    shader.setUniform1b(name + ".isActive", isActive);
}

void TextureMaterial::passTextureToShader(int order, Shader& shader, std::string name) {
    glActiveTexture(GL_TEXTURE0 + order);
    tex.get()->bind();
    shader.setUniform1i(name, order);
}

