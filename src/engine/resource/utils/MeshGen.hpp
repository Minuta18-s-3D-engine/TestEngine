#ifndef ENGINE_ASSETS_UTILS_MESHGEN_H_
#define ENGINE_ASSETS_UTILS_MESHGEN_H_

#include "../../models/Mesh.hpp"
#include "engine/resource/ResourceManager.hpp"

ResourceHandle<Mesh> generateCubeMesh(
    glm::vec3 scale, glm::vec2 textureScale,
    ResourceHandle<MaterialInstance> mat,
    ResourceManager& resourceManager
);

ResourceHandle<Mesh> generateSphereMesh(
    float radius, uint32_t sectors, uint32_t stacks,
    ResourceHandle<MaterialInstance> material,
    ResourceManager& resourceManager
);

#endif // ENGINE_ASSETS_UTILS_MESHGEN_H_
