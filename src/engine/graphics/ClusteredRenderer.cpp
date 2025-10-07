#include "ClusteredRenderer.hpp"

ClusteredRenderer::ClusteredRenderer() : buildClustersShader("buildClusters"),
    lightCullingShader("lightCulling") {
    this->createSSBOs();
}

void ClusteredRenderer::createSSBOs() {
    glGenBuffers(1, &compClusterSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, compClusterSSBO);

    size_t compClusterBufferSize = NUM_CLUSTERS * sizeof(CompCluster);
    glBufferData(
        GL_SHADER_STORAGE_BUFFER, 
        compClusterBufferSize, 
        nullptr, 
        GL_DYNAMIC_DRAW
    );
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, compClusterSSBO);

    glGenBuffers(1, &compLightSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, compLightSSBO);

    size_t compLigthBufferSize = MAX_LIGHTS * sizeof(CompLight);
    glBufferData(
        GL_SHADER_STORAGE_BUFFER, 
        compLigthBufferSize, 
        nullptr, 
        GL_DYNAMIC_DRAW
    );
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, compLightSSBO);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ClusteredRenderer::updateLightData(
    const std::vector<std::shared_ptr<Light>>& lights
) {
    gpuLightsCache.reserve(lights.size());
    
    for (const auto& sceneLight: lights) {
        CompLight gpuLight;
        gpuLight.position = glm::vec4(
            sceneLight->position,
            sceneLight->calcRadius()
        );
        gpuLight.color = glm::vec4(
            sceneLight->color,
            1.0f
        );
        gpuLightsCache.push_back(gpuLight);
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, compLightSSBO);
    glBufferSubData(
        GL_SHADER_STORAGE_BUFFER, 0,
        gpuLightsCache.size() * sizeof(CompLight),
        gpuLightsCache.data()
    );
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ClusteredRenderer::updateClusters(const Camera* cam) {
    buildClustersShader.use();

    buildClustersShader.setUniform1f("zNear", cam->zNear);
    buildClustersShader.setUniform1f("zFar", cam->zFar);

    glm::mat4 proj = glm::perspective(
        cam->getZoom(), 
        (float) Window::width / (float) Window::height, 
        cam->zNear, cam->zFar
    );
    glm::mat4 invProj = glm::inverse(proj);

    buildClustersShader.setUniform4mat("inverseProjection", invProj);
    buildClustersShader.setUniform3ui("gridSize", 
        GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z);
    buildClustersShader.setUniform2ui("screenDimensions", 
        Window::width, Window::height);
    
    glDispatchCompute(GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    lightCullingShader.use();
}
