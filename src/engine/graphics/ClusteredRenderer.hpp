#ifndef ENGINE_GRAPHICS_CLUSTERED_RENDERER_H_
#define ENGINE_GRAPHICS_CLUSTERED_RENDERER_H_

#include "glm/glm.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "../materials/Light.hpp"

struct alignas(16) CompCluster {
    glm::vec4 minPoint;
    glm::vec4 maxPoint;
    uint lightCount;
    uint lightIndicies[100];
};

struct alignas(16) CompLight {
    glm::vec4 position;
    glm::vec4 color;
};

class ClusteredRenderer {
    uint compClusterSSBO;
    uint compLightSSBO;

    std::vector<CompLight> gpuLightsCache;

    // I'm going to move all this settings somewhere, yet don't know where.
    // I'll probably code something like Settings object or smth in future.

    // Size according to DaveH355 recomendations. Not sure if it's best numbers
    // https://github.com/DaveH355/clustered-shading
    const uint GRID_SIZE_X = 12;
    const uint GRID_SIZE_Y = 12;
    const uint GRID_SIZE_Z = 24;
    const uint NUM_CLUSTERS = GRID_SIZE_X * GRID_SIZE_Y * GRID_SIZE_Z;
    const uint MAX_LIGHTS = 2048;

    void createSSBOs();
public:
    ClusteredRenderer();

    void updateLightData(const std::vector<std::shared_ptr<Light>>& lights);
};

#endif // ENGINE_GRAPHICS_CLUSTERED_RENDERER_H_
