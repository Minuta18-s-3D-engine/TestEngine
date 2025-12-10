#ifndef ENGINE_MODELS_MODELLOADER_H_
#define ENGINE_MODELS_MODELLOADER_H_

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include "Model.hpp"
#include "Mesh.hpp"
#include "../assets/coders/images/PngCoder.hpp"
#include "../assets/coders/images/JpgCoder.hpp"
#include "../assets/utils/Files.hpp"
#include "../assets/utils/ImageData.hpp"

class ModelLoader {
    std::unique_ptr<Model> createdModel;
    std::vector<TextureMaterial> loadedTextures;
    std::string directory;
    
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<TextureMaterial> loadMaterialTextures(
        aiMaterial* mat, 
        aiTextureType type, 
        const std::string& typeName,
        const aiScene* scene
    );

    TextureMaterial loadExternalTexture(
        const std::string& path, TextureType type
    );
    
    TextureMaterial loadEmbeddedTexture(
        const aiTexture* embeddedTexture, TextureType texType,
        const std::string& embeddedId
    );

    std::shared_ptr<Texture> loadTexture(
        const uint8_t* imgData,
        size_t imgSize,
        std::string name
    );
public:
    std::unique_ptr<Model> loadModel(const std::string& filename);
};

#endif // ENGINE_MODELS_MODELLOADER_H_
