#include "ClusteredRenderer.hpp"

ClusteredRenderer::ClusteredRenderer(
    Window& _window, AssetManager& _assetManager
) : assetManager(_assetManager), window(_window) {
    this->createSSBOs();
    this->buildClustersShader = 
        &(_assetManager.require<ComputeShader>("shaders/buildClusters"));
    this->lightCullingShader = 
        &(_assetManager.require<ComputeShader>("shaders/lightCulling"));
}

void ClusteredRenderer::createSSBOs() {
    const size_t compClusterBufferSize = NUM_CLUSTERS * sizeof(CompCluster);
    compClusterSSBO.setData(nullptr, compClusterBufferSize);
    compClusterSSBO.bindData();

    const size_t compLightBufferSize = MAX_LIGHTS * sizeof(CompLight);
    compLightSSBO.setData(nullptr, compLightBufferSize);
    compLightSSBO.bindData();

    const size_t compLightIndiciesBufferSize = MAX_LIGHTS_PER_CLUSTER 
        * NUM_CLUSTERS * sizeof(uint32_t);
    compLightIndiciesSSBO.setData(nullptr, compLightIndiciesBufferSize);
    compLightIndiciesSSBO.bindData();

    // 4 would be enough
    const uint32_t MaxDvhDepth = 4;
    const size_t bvhNodesSSBOBufferSize = MAX_LIGHTS * MaxDvhDepth * 
        sizeof(LightBVHWrapper::Node);
    bvhNodesSSBO.setData(nullptr, bvhNodesSSBOBufferSize);
    bvhNodesSSBO.bindData();
    glObjectLabel(GL_BUFFER, bvhNodesSSBO.getId(), -1, "bvh nodes");

    const size_t bvhIndicesSSBOBufferSize = MAX_LIGHTS * MaxDvhDepth * 
        sizeof(uint32_t); // Indices type
    bvhIndicesSSBO.setData(nullptr, bvhIndicesSSBOBufferSize);
    bvhIndicesSSBO.bindData();
    glObjectLabel(GL_BUFFER, bvhIndicesSSBO.getId(), -1, "bvh indices");
}

void ClusteredRenderer::updateLightData(const std::vector<GameObject*>& lights) {
    gpuLightCache.clear();
    gpuLightCache.reserve(lights.size());

    for (int i = 0; i < lights.size(); ++i) {
        CompLight currLight;
        auto pointLightComp = lights[i]->getComponent<PointLight>();
        currLight.position = lights[i]->getComponent<Transform>()->position;
        currLight.color = pointLightComp->color;
        currLight.linear = pointLightComp->linear;
        currLight.quadratic = pointLightComp->quadratic;
        currLight.radius = pointLightComp->calcRadius();

        gpuLightCache.push_back(currLight);
    }

    compLightSSBO.setSubData(
        gpuLightCache.data(), 0, gpuLightCache.size() * sizeof(CompLight)
    );

    LightBVHWrapper bvh(lights);

    std::vector<LightBVHWrapper::Node>& bvhNodes = bvh.getGpuNodes();
    std::vector<size_t>& bvhIndices = bvh.getGpuLightIndicies();
    std::vector<GPUBVHNode> bvhNodesConverted(bvhNodes.size());
    std::vector<uint32_t> bvhIndicesConverted(bvhIndices.size());
    
    for (int i = 0; i < bvhIndicesConverted.size(); ++i) {
        bvhIndicesConverted[i] = (uint32_t) bvhIndices[i];
    }

    for (int i = 0; i < bvhNodesConverted.size(); ++i) {
        auto& node = bvhNodes[i];
        auto& nodeConverted = bvhNodesConverted[i];
        auto bbox = node.get_bbox();

        nodeConverted.maxBounds = glm::vec4(
            bbox.max.values[0], bbox.max.values[1], bbox.max.values[2], 0
        );

        nodeConverted.minBounds = glm::vec4(
            bbox.min.values[0], bbox.min.values[1], bbox.min.values[2], 0
        );
        nodeConverted.first_child_or_primitive = node.index.first_id();
        nodeConverted.primitive_count = node.index.prim_count();
    }

    bvhNodesSSBO.setSubData(
        bvhNodesConverted.data(), 0, 
        bvhNodesConverted.size() * sizeof(GPUBVHNode)
    );

    bvhIndicesSSBO.setSubData(
        bvhIndicesConverted.data(), 0, 
        bvhIndicesConverted.size() * sizeof(uint32_t)
    );
}

void ClusteredRenderer::updateClusters(const Camera* cam) {
    buildClustersShader->use();

    buildClustersShader->setUniform("zNear", cam->zNear);
    buildClustersShader->setUniform("zFar", cam->zFar);
    buildClustersShader->setUniform("currentDispatch", 1);

    glm::mat4 proj = glm::perspective(
        cam->getZoom(), 
        (float) window.getWidth() / (float) window.getHeight(), 
        cam->zNear, cam->zFar
    );
    glm::mat4 invProj = glm::inverse(proj);

    buildClustersShader->setUniform("inverseProjection", invProj);
    buildClustersShader->setUniform("gridSize", 
        glm::uvec3(GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z));
    buildClustersShader->setUniform("screenDimensions", 
        glm::uvec2(window.getWidth(), window.getHeight()));
    
    glDispatchCompute(GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    lightCullingShader->use();

    lightCullingShader->setUniform("viewMat", cam->getViewMat());
    lightCullingShader->setUniform("gridSize",
        glm::uvec3(GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z));
    lightCullingShader->setUniform("currentDispatch", 2);
    lightCullingShader->setUniform("numLights", 
        (uint32_t) this->gpuLightCache.size());
    // lightCullingShader->setUniform1ui("numLights", 2);

    uint numWorkgroups = (NUM_CLUSTERS + LOCAL_SIZE - 1) / LOCAL_SIZE;
    glDispatchCompute(numWorkgroups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

glm::uvec3 ClusteredRenderer::getClusterGrid() {
    return { GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z };
}

void ClusteredRenderer::bindClusterData() {
    compClusterSSBO.bindData();
    compLightSSBO.bindData();
    compLightIndiciesSSBO.bindData();
    bvhNodesSSBO.bindData();
    bvhIndicesSSBO.bindData();
}
