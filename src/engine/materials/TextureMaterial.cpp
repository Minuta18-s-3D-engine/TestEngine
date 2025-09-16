#include "TextureMaterial.hpp"

TextureMaterial::TextureMaterial() : scaleX(1.0f), scaleY(1.0f),
    shiftX(0.0f), shiftY(0.0f), isActive(false) {}

TextureMaterial::TextureMaterial(
    std::shared_ptr<Texture> tex
) : scaleX(1.0f), scaleY(1.0f), tex(tex),
    shiftX(0.0f), shiftY(0.0f), isActive(true) {}

TextureMaterial::TextureMaterial(
    std::shared_ptr<Texture> tex,
    float scaleX, float scaleY,
    float shiftX, float shiftY,
    TextureType type
) : scaleX(scaleX), scaleY(scaleY), tex(tex),
    shiftX(shiftX), shiftY(shiftY), isActive(true), type(type) {}

TextureMaterial::TextureMaterial(
    std::shared_ptr<Texture> tex,
    TextureType type
) : scaleX(1.0), scaleY(1.0), tex(tex),
    shiftX(0.0), shiftY(0.0), isActive(true), type(type) {}

TextureMaterial::~TextureMaterial() {}

void TextureMaterial::passToShader(Shader& shader, std::string name) {
    shader.setUniform1f(name + ".scaleX", scaleX);
    shader.setUniform1f(name + ".scaleY", scaleY);
    shader.setUniform1f(name + ".shiftX", shiftX);
    shader.setUniform1f(name + ".shiftY", shiftY);
    shader.setUniform1b(name + ".isActive", isActive);
}

void TextureMaterial::passToShader(Shader& shader) {}

void TextureMaterial::passTextureToShader(int order, Shader& shader, std::string name) {
    glActiveTexture(GL_TEXTURE0 + order);
    tex.get()->bind();
    shader.setUniform1i(name, order);
}

