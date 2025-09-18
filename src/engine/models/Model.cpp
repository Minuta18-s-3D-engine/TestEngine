#include "Model.hpp"

Model::Model(std::vector<std::shared_ptr<Mesh>> meshes) {
    meshes = meshes;
    wasLoaded = false;
    filename = "";
}

Model::Model(std::string filename) {
    filename = filename;
    wasLoaded = true;

    this->loadModel(filename);
}

void Model::loadModel(std::string filename) {
    // TODO
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    // TODO
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    // TODO
}

std::vector<TextureMaterial> Model::loadMaterialTextures(
    aiMaterial* mat, aiTextureType type, std::string typeName
) {
    // TODO
}

void Model::draw(Shader& shader) {
    for (size_t mesh = 0; mesh < meshes.size(); ++mesh) {
        meshes[mesh]->draw(shader);
    }
}
