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

}
