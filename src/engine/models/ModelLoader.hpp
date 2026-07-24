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
#include "../resource/coders/images/PngCoder.hpp"
#include "../resource/coders/images/JpgCoder.hpp"
#include "../resource/utils/Files.hpp"
#include "../resource/utils/ImageData.hpp"
#include "../project/VirtualPath.hpp"
#include "../resource/ResourceManager.hpp"

class ModelLoader {
    std::unique_ptr<Model> createdModel;
    ResourceHandle<Material> baseMaterialHandle;
    ResourceManager* resourceManager = nullptr;

    std::unordered_map<std::string, ResourceHandle<Texture>> loadedTextures;
    std::unordered_map<int, ResourceHandle<MaterialInstance>> loadedMaterials;

    std::string directory;

    void processNode(aiNode* node, const aiScene* scene);
    ResourceHandle<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
    ResourceHandle<MaterialInstance> loadMaterial(
        aiMaterial* mat,
        uint32_t matIndex,
        const aiScene* scene
    );

    ResourceHandle<Texture> loadExternalTexture(
        const std::string& path
    );

    ResourceHandle<Texture> loadEmbeddedTexture(
        const aiTexture* embeddedTexture,
        const std::string& embeddedId
    );

    ResourceHandle<Texture> loadTexture(
        const uint8_t* imgData,
        size_t imgSize,
        const std::string& name
    );

public:
    std::unique_ptr<Model> loadModel(
        const VirtualPath& filename,
        ResourceHandle<Material> baseMaterial,
        ResourceManager& resourceManager
    );
};

#endif // ENGINE_MODELS_MODELLOADER_H_
