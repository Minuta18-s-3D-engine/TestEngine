#ifndef ENGINE_MODELS_MODEL_H_
#define ENGINE_MODELS_MODEL_H_

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>
#include <memory>

#include "Mesh.hpp"

class Model {
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::string filename;
    bool wasLoaded;
    
    void loadModel(std::string filename);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<TextureMaterial> loadMaterialTextures(
        aiMaterial* mat, aiTextureType type, std::string typeName
    );
public:
    Model(std::vector<std::shared_ptr<Mesh>> meshes);
    Model(std::string filename);

    void draw(Shader& shader);
};

#endif // ENGINE_MODELS_MODEL_H_
