#ifndef ENGINE_MODELS_MODELLOADER_H_
#define ENGINE_MODELS_MODELLOADER_H_

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>
#include <iostream>

#include "Model.hpp"
#include "Mesh.hpp"
#include "../assets/coders/images/PngCoder.hpp"
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
        std::string typeName
    );
public:
    std::unique_ptr<Model>& loadModel(std::string filename);
};

#endif // ENGINE_MODELS_MODELLOADER_H_
