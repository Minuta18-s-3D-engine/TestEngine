#include "Model.hpp"

Model::Model(ResourceManager &resourceManager_, MeshArray meshes_)
    : resourceManager(&resourceManager_), meshes(std::move(meshes_)) {}

Model::Model(ResourceManager& resourceManager_) : resourceManager(&resourceManager_) {}

void Model::draw() const {
    for (auto& meshHandle : meshes) {
        Mesh& mesh = resourceManager->require(meshHandle);
        mesh.draw();
    }
}
