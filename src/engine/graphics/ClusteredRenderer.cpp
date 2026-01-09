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
        bvhNodesSSBOBufferSize,
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
        bvhIndicesSSBOBufferSize,
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

void ClusteredRenderer::updateLightData(const std::vector<GameObject*>& lights) {
    gpuLightCache.clear();
    gpuLightCache.reserve(lights.size());

    for (int i = 0; i < lights.size(); ++i) {
        CompLight currLight;
        currLight.position = lights[i]->getComponent<Transform>()->position;
        currLight.color = lights[i]->getComponent<PointLight>()->color;
        currLight.linear = lights[i]->getComponent<PointLight>()->linear;
        currLight.quadratic = lights[i]->getComponent<PointLight>()->quadratic;
        currLight.radius = lights[i]->getComponent<PointLight>()->calcRadius();

        gpuLightCache.push_back(currLight);
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, compLightSSBO);
    glBufferData(
        GL_SHADER_STORAGE_BUFFER,
        gpuLightCache.size() * sizeof(CompLight),
        gpuLightCache.data(),
        GL_DYNAMIC_COPY
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


    glBindBuffer(GL_SHADER_STORAGE_BUFFER, bvhNodesSSBO);
    glBufferData(
        GL_SHADER_STORAGE_BUFFER,
        bvhNodesConverted.size() * sizeof(GPUBVHNode),
        bvhNodesConverted.data(),
        GL_DYNAMIC_DRAW
    );

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, bvhIndicesSSBO);
    glBufferData(
        GL_SHADER_STORAGE_BUFFER,
        bvhIndicesConverted.size() * sizeof(uint32_t),
        bvhIndicesConverted.data(),
        GL_DYNAMIC_DRAW
    );

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ClusteredRenderer::updateClusters(const Camera* cam) {
    buildClustersShader->use();

    buildClustersShader->setUniform1f("zNear", cam->zNear);
    buildClustersShader->setUniform1f("zFar", cam->zFar);
    buildClustersShader->setUniform1i("currentDispatch", 1);

    glm::mat4 proj = glm::perspective(
        cam->getZoom(), 
        (float) window.getWidth() / (float) window.getHeight(), 
        cam->zNear, cam->zFar
    );
    glm::mat4 invProj = glm::inverse(proj);

    buildClustersShader->setUniform4mat("inverseProjection", invProj);
    buildClustersShader->setUniform3ui("gridSize", 
        GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z);
    buildClustersShader->setUniform2ui("screenDimensions", 
        window.getWidth(), window.getHeight());
    
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
    glBindBufferBase(
        GL_SHADER_STORAGE_BUFFER, 
        SSBOBindings::BVH_NODES, 
        bvhNodesSSBO
    );
    glBindBufferBase(
        GL_SHADER_STORAGE_BUFFER,
        SSBOBindings::BVH_INDICES,
        bvhIndicesSSBO
    );
}
