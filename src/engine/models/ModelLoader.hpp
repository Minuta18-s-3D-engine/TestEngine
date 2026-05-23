#ifndef ENGINE_MODELS_MODELLOADER_H_
#define ENGINE_MODELS_MODELLOADER_H_

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "Model.hpp"
#include "Mesh.hpp"
#include "../materials/Material.hpp"
#include "../assets/coders/images/PngCoder.hpp"
#include "../assets/coders/images/JpgCoder.hpp"
#include "../assets/utils/Files.hpp"
#include "../assets/utils/ImageData.hpp"
#include "../project/VirtualPath.hpp"

class ModelLoader {
    std::unique_ptr<Model> createdModel;
    std::shared_ptr<Material> baseMaterial;
    std::unordered_map<std::string, std::shared_ptr<Texture>> loadedTextures;
    std::unordered_map<int, std::shared_ptr<MaterialInstance>> loadedMaterials;

    std::string directory;
    
    void processNode(aiNode* node, const aiScene* scene);
    std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
    std::shared_ptr<MaterialInstance> loadMaterial(
        aiMaterial* mat, 
        uint32_t matIndex,
        const aiScene* scene
    );

    std::shared_ptr<Texture> loadExternalTexture(
        const std::string& path
    );
    
    std::shared_ptr<Texture> loadEmbeddedTexture(
        const aiTexture* embeddedTexture, const std::string& embeddedId
    );

    std::shared_ptr<Texture> loadTexture(
        const uint8_t* imgData,
        size_t imgSize,
        std::string name
    );
public:
    std::unique_ptr<Model> loadModel(
        const VirtualPath& filename,
        std::shared_ptr<Material> baseMaterial
    );
};

#endif // ENGINE_MODELS_MODELLOADER_H_
