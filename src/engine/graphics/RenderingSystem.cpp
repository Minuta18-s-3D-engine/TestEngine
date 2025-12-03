#include "RenderingSystem.hpp"

RenderingSystem::RenderingSystem(
    AssetManager& _assetManager,
    GameObjectManager& _gameObjectManager 
    ): assetManager(_assetManager), gameObjectManager(_gameObjectManager) {
    renderer = new ClusteredRenderer(_assetManager);
    gBuffer = new GBuffer(Window::width, Window::height);

    Window::addframebufferCallback([&] (GLFWwindow* win, int width, int height) {
        gBuffer->resize(width, height);
    });
}

RenderingSystem::~RenderingSystem() {
    delete gBuffer;
    delete renderer;
}

void RenderingSystem::setDrawMode(uint newDrawMode) {
    this->drawMode = newDrawMode;
}

void RenderingSystem::bindCamera(Camera* camera) {
    this->camera = camera;
}

void RenderingSystem::render() {
    // camera is not bound, its possible to place here some effect in future
    if (camera == nullptr) {
        return;
    }

    // TODO: add cache
    std::vector<GameObject*> lights = 
        gameObjectManager.getObjectsWithComponents<Transform, Behavior, PointLight>();
    renderer->updateLightData(lights);
    renderer->updateClusters(camera);
    
    Shader& geomShader = assetManager.require<Shader>("shaders/geomShader");
    Shader& lightingShader = assetManager.require<Shader>("shaders/lightingShader");

    gBuffer->bind();
    
    glm::mat4 proj = glm::mat4(1.0f);
    proj = glm::perspective(
        camera->getZoom(), 
        (float) Window::width / (float) Window::height, 
        camera->zNear, camera->zFar
    );
    glm::mat4 viewMat = camera->getViewMat();
    glm::mat4 worldModel = glm::mat4(1.0f);

    std::vector<GameObject*> objects = 
        gameObjectManager.getObjectsWithComponents<Transform>();

    geomShader.use();
    geomShader.setUniform4mat("projection", proj);
    geomShader.setUniform4mat("view", viewMat);
    for (auto& object : objects) {
        auto transformComponent = object->getComponent<Transform>();
        glm::mat4 model = glm::translate(
            worldModel, transformComponent->position);

        geomShader.setUniform4mat("model", model);
        // object->draw(geomShader); // TODO
    }

    gBuffer->unbind();
}