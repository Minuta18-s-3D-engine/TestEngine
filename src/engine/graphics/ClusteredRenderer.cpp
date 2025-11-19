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
    // I really need an createSSBO() function

    glGenBuffers(1, &compClusterSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, compClusterSSBO);
    size_t compClusterBufferSize = NUM_CLUSTERS * sizeof(CompCluster);
    glBufferData(
        GL_SHADER_STORAGE_BUFFER, 
        compClusterBufferSize, 
        nullptr, 
        GL_STATIC_COPY
    );
    glBindBufferBase(
        GL_SHADER_STORAGE_BUFFER, 
        SSBOBindings::CLUSTER_BINDING, 
        compClusterSSBO
    );

    glGenBuffers(1, &compLightSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, compLightSSBO);
    size_t compLigthBufferSize = MAX_LIGHTS * sizeof(CompLight);
    glBufferData(
        GL_SHADER_STORAGE_BUFFER, 
        compLigthBufferSize, 
        nullptr, 
        GL_DYNAMIC_COPY
    );
    glBindBufferBase(
        GL_SHADER_STORAGE_BUFFER, 
        SSBOBindings::LIGHT_BINDING, 
        compLightSSBO
    );
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glGenBuffers(1, &compLightIndiciesSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, compLightIndiciesSSBO);
    size_t compLightIndiciesBufferSize = MAX_LIGHTS_PER_CLUSTER 
        * NUM_CLUSTERS * sizeof(uint);
    glBufferData(
        GL_SHADER_STORAGE_BUFFER,
        compLightIndiciesBufferSize,
        nullptr,
        GL_DYNAMIC_COPY
    );
    glBindBufferBase(
        GL_SHADER_STORAGE_BUFFER,
        SSBOBindings::LIGHT_INDICIES_BINDING,
        compLightIndiciesSSBO
    );
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glGenBuffers(1, &bvhNodesSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, bvhNodesSSBO);
    // 4 would be enough for any type of tree, especially for binary trees/bvhs
    constexpr uint MaxDvhDepth = 4;
    size_t bvhNodesSSBOBufferSize = MAX_LIGHTS * MaxDvhDepth * 
        sizeof(LightBVHWrapper::Node);
    glBufferData(
        GL_SHADER_STORAGE_BUFFER,
        bvhNodesSSBO,
        nullptr,
        GL_DYNAMIC_COPY
    );
    glBindBufferBase(
        GL_SHADER_STORAGE_BUFFER,
        SSBOBindings::BVH_NODES,
        bvhNodesSSBO
    );
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glGenBuffers(1, &bvhIndicesSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, bvhIndicesSSBO);
    size_t bvhIndicesSSBOBufferSize = MAX_LIGHTS * MaxDvhDepth * 
        sizeof(uint); // Indices type
    glBufferData(
        GL_SHADER_STORAGE_BUFFER,
        bvhIndicesSSBO,
        nullptr,
        GL_DYNAMIC_COPY
    );
    glBindBufferBase(
        GL_SHADER_STORAGE_BUFFER,
        SSBOBindings::BVH_INDICES,
        bvhIndicesSSBO
    );
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ClusteredRenderer::updateLightData(
    const std::vector<std::shared_ptr<Light>>& lights
) {
    gpuLightCache.clear();
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
    glBufferData(
        GL_SHADER_STORAGE_BUFFER,
        gpuLightCache.size() * sizeof(CompLight),
        gpuLightCache.data(),
        GL_DYNAMIC_COPY
    );

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // glBindBuffer(GL_SHADER_STORAGE_BUFFER, compLightSSBO);
    // void* mappedData = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, gpuLightCache.size() * sizeof(CompLight), GL_MAP_READ_BIT);
    // if (mappedData) {
    //     CompLight* lightedData = static_cast<CompLight*>(mappedData);

    //     for (int i = 0; i < gpuLightCache.size(); ++i) {
    //         std::cout << "Light " << i + 1 << ": color=" <<
    //             lightedData[i].color.r << ";" <<
    //             lightedData[i].color.g << ";" <<
    //             lightedData[i].color.b << " pos=" <<
    //             lightedData[i].position.r << ";" <<
    //             lightedData[i].position.g << ";" <<
    //             lightedData[i].position.b << " rad=" <<
    //             lightedData[i].radius << std::endl;
    //     }

    //     glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    // } else {
    //     std::cout << "No data was written!" << std::endl;
    // }
    // std::cout << "=========================" << std::endl;
    // glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ClusteredRenderer::updateClusters(const Camera* cam) {
    buildClustersShader->use();

    buildClustersShader->setUniform1f("zNear", cam->zNear);
    buildClustersShader->setUniform1f("zFar", cam->zFar);
    buildClustersShader->setUniform1i("currentDispatch", 1);

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
    lightCullingShader->setUniform3ui("gridSize",
        GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z);
    lightCullingShader->setUniform1i("currentDispatch", 2);
    lightCullingShader->setUniform1ui("numLights", this->gpuLightCache.size());
    // lightCullingShader->setUniform1ui("numLights", 2);

    uint numWorkgroups = (NUM_CLUSTERS + LOCAL_SIZE - 1) / LOCAL_SIZE;
    glDispatchCompute(numWorkgroups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

glm::uvec3 ClusteredRenderer::getClusterGrid() {
    return { GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z };
}

void ClusteredRenderer::bindClusterData() {
    glBindBufferBase(
        GL_SHADER_STORAGE_BUFFER, 
        SSBOBindings::CLUSTER_BINDING, 
        compClusterSSBO
    );
    glBindBufferBase(
        GL_SHADER_STORAGE_BUFFER, 
        SSBOBindings::LIGHT_BINDING, 
        compLightSSBO
    );
    glBindBufferBase(
        GL_SHADER_STORAGE_BUFFER,
        SSBOBindings::LIGHT_INDICIES_BINDING,
        compLightIndiciesSSBO
    );
}
