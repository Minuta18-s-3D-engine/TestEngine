#ifndef ENGINE_ASSETS_UTILS_MESHGEN_H_
#define ENGINE_ASSETS_UTILS_MESHGEN_H_

#include "../../models/Mesh.hpp"
#include "engine/assets/AssetManager.hpp"

std::shared_ptr<Mesh> generateCubeMesh(
    glm::vec3 scale, glm::vec2 textureScale,
    std::string textureKey, AssetManager& manager, Material mat
);

#endif // ENGINE_ASSETS_UTILS_MESHGEN_H_
