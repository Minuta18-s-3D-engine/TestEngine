#ifndef ENGINE_MATERIAL_TEXTUREMATERIAL_H_
#define ENGINE_MATERIAL_TEXTUREMATERIAL_H_

#include "glm/glm.hpp"
#include "../graphics/Shader.hpp"
#include "BaseMaterial.hpp"
#include "../graphics/Texture.hpp"
#include <memory>
#include <string>

enum TextureType {
    DIFFUSE,
    SPECULAR
};

class TextureMaterial : public BaseMaterial {
public:
    float scaleX, scaleY;
    float shiftX, shiftY;
    bool isActive;
    std::shared_ptr<Texture> tex;
    TextureType type;

    TextureMaterial();
    TextureMaterial(std::shared_ptr<Texture> tex);
    TextureMaterial(
        std::shared_ptr<Texture> tex,
        float scaleX, float scaleY,
        float shiftX, float shiftY, 
        TextureType type
    );
    TextureMaterial(
        std::shared_ptr<Texture> tex,
        TextureType type
    );
    ~TextureMaterial();

    void passToShader(Shader& shader);
    void passToShader(Shader& shader, std::string name);
    void passTextureToShader(int order, Shader& shader, std::string name);
};

#endif //  ENGINE_MATERIAL_TEXTUREMATERIAL_H_
