#include "ClusteredRenderer.hpp"

ClusteredRenderer::ClusteredRenderer(AssetManager& _assetManager) : 
    assetManager(_assetManager) {
    this->createSSBOs();
    this->buildClustersShader = 
        _assetManager.get<ComputeShader>("shaders/buildClusters");
    this->lightCullingShader = 
        _assetManager.get<ComputeShader>("shaders/lightCulling");
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
    gpuLightCache.reserve(lights.size());

    for (int i = 0; i < lights.size(); ++i) {
        CompLight currLight;
        currLight.position = lights[i]->position;
        currLight.color = lights[i]->color;
        currLight.linear = lights[i]->linear;
        currLight.quadratic = lights[i]->quadratic;
        currLight.radius = lights[i]->calcRadius();

        gpuLightCache.push_back(currLight);
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, compLightSSBO);
    glBufferSubData(
        GL_SHADER_STORAGE_BUFFER, 0,
        gpuLightCache.size() * sizeof(CompLight),
        gpuLightCache.data()
    );
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ClusteredRenderer::updateClusters(const Camera* cam) {
    buildClustersShader->use();

    buildClustersShader->setUniform1f("zNear", cam->zNear);
    buildClustersShader->setUniform1f("zFar", cam->zFar);

    glm::mat4 proj = glm::perspective(
        cam->getZoom(), 
        (float) Window::width / (float) Window::height, 
        cam->zNear, cam->zFar
    );
    glm::mat4 invProj = glm::inverse(proj);

    buildClustersShader->setUniform4mat("inverseProjection", invProj);
    buildClustersShader->setUniform3ui("gridSize", 
        GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z);
    buildClustersShader->setUniform2ui("screenDimensions", 
        Window::width, Window::height);
    
    glDispatchCompute(GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    lightCullingShader->use();

    lightCullingShader->setUniform4mat("viewMat", cam->getViewMat());
    lightCullingShader->setUniform1i("numLights", gpuLightCache.size());

    uint numWorkgroups = (NUM_CLUSTERS + LOCAL_SIZE - 1) / LOCAL_SIZE;
    glDispatchCompute(numWorkgroups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
