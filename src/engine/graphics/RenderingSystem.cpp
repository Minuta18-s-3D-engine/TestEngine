#include "RenderingSystem.hpp"

RenderingSystem::RenderingSystem(AssetManager& _assetManager) 
    : assetManager(_assetManager) {
    renderer = new ClusteredRenderer(_assetManager);
    gBuffer = new GBuffer(Window::width, Window::height);

    Window::addframebufferCallback([&] (GLFWwindow* win, int width, int height) {
        gBuffer->resize(width, height);
    });
}

void RenderingSystem::update() {

}