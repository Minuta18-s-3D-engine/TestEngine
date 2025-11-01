#ifndef ENGINE_SCENE_SCENE_H_
#define ENGINE_SCENE_SCENE_H_

#include "SceneObject.hpp"
#include "../assets/AssetManager.hpp"
#include "../graphics/Camera.hpp"
#include "../window/Window.hpp"
#include "../materials/Light.hpp"
#include "../graphics/ClusteredRenderer.hpp"
#include "../graphics/GBuffer.hpp"

#include <chrono>
#include <memory>
#include <vector>
#include <iostream>

class Scene {
    AssetManager& assetManager;

    std::vector<std::shared_ptr<SceneObject>> objects;
    std::vector<std::shared_ptr<Light>> lights; // temporary solution

    ClusteredRenderer* renderer;
    GBuffer* gBuffer;

    uint quadVAO = 0, quadVBO; // I'll move this to mesh after i'll be sure 
                               // GBuffer works.
    uint drawMode = 0;

    void renderQuad();
    bool firstRender;

    void debugDraw(Camera* cam);
public:
    Scene(AssetManager& _assetManager);
    ~Scene();

    void addObject(std::shared_ptr<SceneObject> obj);
    void addLight(std::shared_ptr<Light> obj);
    void drawAll(Camera* cam); // Move this to RenderSystem as fast as possible
    void setDrawMode(uint newDrawMode);
};

#endif // ENGINE_SCENE_SCENE_H_
