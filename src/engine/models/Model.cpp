#include "Model.hpp"

Model::Model(std::vector<std::shared_ptr<Mesh>>& meshes) {
    this->meshes = meshes;
}

void Model::draw(Shader& shader) {
    for (size_t mesh = 0; mesh < meshes.size(); ++mesh) {
        meshes[mesh]->draw(shader);
    }
}
