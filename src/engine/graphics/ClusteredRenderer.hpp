#ifndef ENGINE_GRAPHICS_CLUSTERED_RENDERER_H_
#define ENGINE_GRAPHICS_CLUSTERED_RENDERER_H_

#include "glm/glm.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "../materials/old/Light.hpp"
#include "../assets/AssetManager.hpp"
#include "ComputeShader.hpp"
#include "Camera.hpp"
#include "ShaderStorageBuffer.hpp"
#include "../window/Window.hpp"
#include "LightBVHWrapper.hpp"
#include "../gameObject/GameObject.hpp"
#include "../gameObject/GameObjectManager.hpp"
#include "../gameObject/components/Transform.hpp"
#include "../gameObject/components/Behavior.hpp"
#include "components/PointLight.hpp"

struct alignas(16) CompCluster {
    glm::vec4 minPoint;
    glm::vec4 maxPoint;
    uint lightCount;
    uint lightStart;
};

struct alignas(16) CompLight {
    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 color;
    float linear, quadratic, radius;
};

class ClusteredRenderer {
    ShaderStorageBuffer compClusterSSBO{
        SSBOBindings::CLUSTER_BINDING, GL_STATIC_DRAW};
    ShaderStorageBuffer compLightSSBO{
        SSBOBindings::LIGHT_BINDING, GL_DYNAMIC_DRAW};
    ShaderStorageBuffer compLightIndiciesSSBO{
        SSBOBindings::LIGHT_INDICIES_BINDING, GL_DYNAMIC_DRAW};
    ShaderStorageBuffer bvhNodesSSBO{
        SSBOBindings::BVH_NODES, GL_DYNAMIC_DRAW};
    ShaderStorageBuffer bvhIndicesSSBO{
        SSBOBindings::BVH_INDICES, GL_DYNAMIC_DRAW};

    const uint MAX_LIGHTS_PER_CLUSTER = 256;

    ComputeShader *buildClustersShader, *lightCullingShader;
    AssetManager& assetManager;
    Window& window;
    
    std::vector<CompLight> gpuLightCache;

    // I'm going to move all this settings somewhere, yet don't know where.
    // I'll probably code something like Settings object or smth in future.

    // Size according to DaveH355 recommendations. Not sure if it's best numbers
    // https://github.com/DaveH355/clustered-shading
    const uint32_t GRID_SIZE_X = 12;
    const uint32_t GRID_SIZE_Y = 12;
    const uint32_t GRID_SIZE_Z = 24;
    const uint32_t NUM_CLUSTERS = GRID_SIZE_X * GRID_SIZE_Y * GRID_SIZE_Z;
    const uint32_t MAX_LIGHTS = 2048;
    const uint32_t LOCAL_SIZE = 128;

    void createSSBOs();
public:
    ClusteredRenderer(Window& _window, AssetManager& _assetManager);
    ~ClusteredRenderer() = default;

    void updateLightData(const std::vector<std::shared_ptr<Light>>& lights);
    // Assume only dynamic lights are given
    void updateLightData(const std::vector<GameObject*>& lights);
    void updateClusters(const Camera* cam);
    glm::uvec3 getClusterGrid();
    void bindClusterData();
};

#endif // ENGINE_GRAPHICS_CLUSTERED_RENDERER_H_
