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

void RenderingSystem::update() {
    // camera is not bound, its possible to place here some effect in future
    if (camera == nullptr) {
        return;
    }

    // TODO: add cache
    std::vector<GameObject*> lights = 
        gameObjectManager.getObjectsWithComponents<Transform, Behavior, PointLight>();
}