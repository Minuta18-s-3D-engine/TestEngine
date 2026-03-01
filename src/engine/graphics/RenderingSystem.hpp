#ifndef ENGINE_GRAPHICS_RENDERINGSYSTEM_H_
#define ENGINE_GRAPHICS_RENDERINGSYSTEM_H_

#include <memory>
#include <vector>
#include <chrono>

#include "../gameObject/System.hpp"
#include "../gameObject/GameObject.hpp"
#include "../gameObject/GameObjectManager.hpp"
#include "../gameObject/components/Transform.hpp"
#include "../gameObject/components/Behavior.hpp"
#include "../models/ModelComponent.hpp"
#include "components/PointLight.hpp"
#include "../models/Model.hpp"

#include "../assets/AssetManager.hpp"
#include "../graphics/ClusteredRenderer.hpp"
#include "../graphics/GBuffer.hpp"
#include "../graphics/Camera.hpp"
#include "../window/Window.hpp"
#include "../window/WindowEvents.hpp"
#include "../events/EventManager.hpp"

class RenderingSystem : public System {
    AssetManager& assetManager;
    GameObjectManager& gameObjectManager;
    Window& window;
    EventManager& eventManager;

    ClusteredRenderer* renderer;
    GBuffer* gBuffer;
    Camera* camera = nullptr;

    std::vector<GameObject*> lightCache, objectsCache;

    uint drawMode = 0;

    uint quadVAO = 0, quadVBO;
    void renderQuad();
    void onWindowResize(WindowResizeEvent& event);
public:
    RenderingSystem(
        AssetManager& _assetManager, 
        GameObjectManager& _gameObjectManager,
        EventManager& _eventManager,
        Window& _window
    );
    ~RenderingSystem();

    void update() override;
    void render();
    void bindCamera(Camera* camera);
    void setDrawMode(uint newDrawMode);
    void updateCache();
};

#endif // ENGINE_GRAPHICS_RENDERINGSYSTEM_H_