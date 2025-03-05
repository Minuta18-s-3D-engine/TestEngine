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
    float shiftX, float shiftY
) : scaleX(scaleX), scaleY(scaleY), tex(tex),
    shiftX(shiftX), shiftY(shiftY), isActive(true) {}

TextureMaterial::~TextureMaterial() {}

void TextureMaterial::passToShader(Shader& shader, std::string name) {
    shader.setUniform1f(name + ".scaleX", scaleX);
    shader.setUniform1f(name + ".scaleY", scaleY);
    shader.setUniform1f(name + ".shiftX", shiftX);
    shader.setUniform1f(name + ".shiftY", shiftY);
    shader.setUniform1b(name + ".isActive", isActive);
}

void TextureMaterial::passToShader(Shader& shader) {}

void TextureMaterial::passTextureToShader(GLenum order) {
    glActiveTexture(order);
    tex.get()->bind();
}

