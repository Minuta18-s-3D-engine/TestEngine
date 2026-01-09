#include "RenderingSystem.hpp"

RenderingSystem::RenderingSystem(
    AssetManager& _assetManager,
    GameObjectManager& _gameObjectManager, 
    ModelManager& _modelManager,
    EventManager& _eventManager,
    Window& _window
): assetManager(_assetManager), gameObjectManager(_gameObjectManager),
    modelManager(_modelManager), window(_window), eventManager(_eventManager) {
    renderer = new ClusteredRenderer(window, _assetManager);
    gBuffer = new GBuffer(window.getWidth(), window.getWidth());

    eventManager.subscribe<WindowResizeEvent>(
        &RenderingSystem::onWindowResize);
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

void RenderingSystem::render() {
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
    geomShader.setUniform4mat("projection", proj);
    geomShader.setUniform4mat("view", viewMat);
    for (auto& object : objectsCache) {
        auto transformComponent = object->getComponent<Transform>();
        auto modelComponent = object->getComponent<ModelComponent>();
        glm::mat4 model = glm::translate(
            worldModel, transformComponent->position);

        geomShader.setUniform4mat("model", model);
        auto objModel = modelManager.get(modelComponent->managerId);
        objModel->draw(geomShader); 
    }

    gBuffer->unbind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lightingShader.use();

    gBuffer->bindBufffers();
    lightingShader.setUniform1i("gPosition", 0);
    lightingShader.setUniform1i("gNormal", 1);
    lightingShader.setUniform1i("gAlbedoSpec", 2);

    lightingShader.setUniform1ui("drawMode", drawMode);
    lightingShader.setUniform3f("viewPos", camera->pos);
    lightingShader.setUniform1f("zNear", camera->zNear);
    lightingShader.setUniform1f("zFar", camera->zFar);
    lightingShader.setUniform4mat("projection", proj);
    lightingShader.setUniform4mat("view", viewMat);
    lightingShader.setUniform3ui("gridSize", renderer->getClusterGrid());
    lightingShader.setUniform2ui("screenDimensions", 
        window.getWidth(), window.getHeight());

    renderer->bindClusterData();

    renderQuad();
}

void RenderingSystem::update() {
    this->render();
}
