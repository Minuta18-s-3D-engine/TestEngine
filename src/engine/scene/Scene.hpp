#ifndef ENGINE_SCENE_SCENE_H_
#define ENGINE_SCENE_SCENE_H_

#include "SceneObject.hpp"
#include "../assets/AssetManager.hpp"
#include "../graphics/Camera.hpp"
#include "../window/Window.hpp"
#include "../materials/Light.hpp"
#include "../graphics/GBuffer.hpp"

#include <memory>
#include <vector>

class Scene {
    AssetManager& assetManager;

    std::vector<std::shared_ptr<SceneObject>> objects;
    std::vector<std::shared_ptr<Light>> lights; // temporary solution
    GBuffer* gBuffer; // since rendering and scene temproray merged, I'll put
                      // it here until separate Graphics class will appear

    uint quadVAO = 0, quadVBO; // I'll move this to mesh after i'll be sure 
                               // GBuffer works.
    void renderQuad();
public:
    Scene(AssetManager& _assetManager);
    ~Scene();

    void addObject(std::shared_ptr<SceneObject> obj);
    void addLight(std::shared_ptr<Light> obj);
    void drawAll(Camera* cam); // Move this to RenderSystem as fast as possible
};

#endif // ENGINE_SCENE_SCENE_H_
