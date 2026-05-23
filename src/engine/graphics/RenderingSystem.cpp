#include "RenderingSystem.hpp"

RenderingSystem::RenderingSystem(
    AssetManager& _assetManager,
    GameObjectManager& _gameObjectManager, 
    EventManager& _eventManager,
    Window& _window,
    MaterialDataBuffer& _globalMaterialBuffer,
): assetManager(_assetManager), gameObjectManager(_gameObjectManager),
    window(_window), eventManager(_eventManager),
    globalMaterialBuffer(_globalMaterialBuffer) {
    renderer = new ClusteredRenderer(window, _assetManager);
    gBuffer = new GBuffer(window.getWidth(), window.getHeight());

    eventManager.subscribe<WindowResizeEvent>(
        this, &RenderingSystem::onWindowResize);
}

RenderingSystem::~RenderingSystem() {
    delete gBuffer;
    delete renderer;
}

void RenderingSystem::onWindowResize(WindowResizeEvent& event) {
    gBuffer->resize(event.newWidth, event.newHeight);
}

void RenderingSystem::setDrawMode(uint newDrawMode) {
    this->drawMode = newDrawMode;
}

void RenderingSystem::bindCamera(Camera* camera) {
    this->camera = camera;
}
void RenderingSystem::renderQuad() {
    if (quadVAO == 0) {
        float quadVertices[] = {
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void RenderingSystem::updateCache() {
    lightCache = gameObjectManager.getObjectsWithComponents
        <Transform, Behavior, PointLight>();
    objectsCache = gameObjectManager.getObjectsWithComponents
        <Transform, ModelComponent, Behavior>();
}

void RenderingSystem::render(float deltaTime) {
    time += deltaTime;
    currentFrame++;

    // camera is not bound, its possible to place here some effect in future
    if (camera == nullptr) {
        return;
    }
    
    renderer->updateLightData(lightCache);
    renderer->updateClusters(camera);
    
    Shader& geomShader = assetManager.require<Shader>("shaders/geomShader");
    Shader& lightingShader = assetManager.require<Shader>("shaders/lightingShader");

    gBuffer->bind();
    
    glm::mat4 proj = glm::mat4(1.0f);
    proj = glm::perspective(
        camera->getZoom(), 
        (float) window.getWidth() / (float) window.getHeight(), 
        camera->zNear, camera->zFar
    );
    glm::mat4 viewMat = camera->getViewMat();
    glm::mat4 worldModel = glm::mat4(1.0f);

    geomShader.use();

    globalMaterialBuffer.bind();

    geomShader.setUniform("u_Time", time);
    geomShader.setUniform("u_DeltaTime", deltaTime);
    geomShader.setUniform("u_Frame", currentFrame);

    glm::ivec2 resolution = glm::ivec2(window.getWidth(), window.getHeight());
    geomShader.setUniform("u_Resolution", resolution);
    geomShader.setUniform("u_TexelSize", 
        glm::vec2(1.0f / resolution.x, 1.0f / resolution.y));
    
    geomShader.setUniform("u_CameraPosition", camera->pos);
    geomShader.setUniform("u_CameraDirection", camera->front);

    geomShader.setUniform("u_View", viewMat);
    geomShader.setUniform("u_Projection", proj);
    geomShader.setUniform("u_InvView", glm::inverse(viewMat));
    geomShader.setUniform("u_InvProjection", glm::inverse(proj));

    geomShader.setUniform("u_ZNear", camera->zNear);
    geomShader.setUniform("u_ZFar", camera->zFar);  

    for (auto& object : objectsCache) {
        auto transformComponent = object->getComponent<Transform>();
        auto modelComponent = object->getComponent<ModelComponent>();
        glm::mat4 model = glm::translate(
            worldModel, transformComponent->position);

        geomShader.setUniform("u_Model", model);
        geomShader.setUniform("u_InvModel", glm::inverse(model));
        auto objModel = assetManager.get<Model>(modelComponent->managerId);
        if (!objModel) {
            continue;
        }
        objModel->draw(geomShader); 
    }

    gBuffer->unbind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lightingShader.use();

    gBuffer->bindBuffers();
    lightingShader.setUniform("u_gPosition", 0);
    lightingShader.setUniform("u_gNormal", 1);
    lightingShader.setUniform("u_gAlbedoSpec", 2);

    lightingShader.setUniform("u_Time", time);
    lightingShader.setUniform("u_DeltaTime", deltaTime);
    lightingShader.setUniform("u_Frame", currentFrame);

    glm::ivec2 resolution = glm::ivec2(window.getWidth(), window.getHeight());
    lightingShader.setUniform("u_Resolution", resolution);
    lightingShader.setUniform("u_TexelSize", 
        glm::vec2(1.0f / resolution.x, 1.0f / resolution.y));
    
    lightingShader.setUniform("u_CameraPosition", camera->pos);
    lightingShader.setUniform("u_CameraDirection", camera->front);

    lightingShader.setUniform("u_View", viewMat);
    lightingShader.setUniform("u_Projection", proj);
    lightingShader.setUniform("u_InvView", glm::inverse(viewMat));
    lightingShader.setUniform("u_InvProjection", glm::inverse(proj));

    lightingShader.setUniform("u_ZNear", camera->zNear);
    lightingShader.setUniform("u_ZFar", camera->zFar);  


    renderer->bindClusterData();

    renderQuad();
}

void RenderingSystem::update() {
    this->render();
}
