#ifndef ENGINE_ASSETS_UTILS_MESHGEN_H_
#define ENGINE_ASSETS_UTILS_MESHGEN_H_

#include "../../models/Mesh.hpp"
#include "engine/assets/AssetManager.hpp"

std::shared_ptr<Mesh> generateCubeMesh(
    glm::vec3 scale, glm::vec2 textureScale,
    std::shared_ptr<MaterialInstance> mat
);

std::shared_ptr<Mesh> generateSphereMesh(
    float radius, uint32_t sectors, uint32_t stacks,
    std::shared_ptr<MaterialInstance> material
);

#endif // ENGINE_ASSETS_UTILS_MESHGEN_H_
