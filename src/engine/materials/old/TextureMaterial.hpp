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
    SPECULAR,
    NORMAL,
    HEIGHT
};

class TextureMaterial : public BaseMaterial {
public:
    bool isActive;
    std::shared_ptr<Texture> tex;
    std::string filename = "";
    TextureType type;

    TextureMaterial();
    TextureMaterial(std::shared_ptr<Texture> tex);
    TextureMaterial(
        std::shared_ptr<Texture> tex,
        TextureType type
    );
    ~TextureMaterial();

    void passToShader(Shader& shader, std::string name);
    void passTextureToShader(int order, Shader& shader, std::string name);
};

#endif //  ENGINE_MATERIAL_TEXTUREMATERIAL_H_
