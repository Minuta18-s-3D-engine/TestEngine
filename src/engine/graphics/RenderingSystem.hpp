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

#include "../graphics/ClusteredRenderer.hpp"
#include "../graphics/GBuffer.hpp"
#include "../graphics/Camera.hpp"
#include "../window/Window.hpp"
#include "../window/WindowEvents.hpp"
#include "../events/EventManager.hpp"
#include "../materials/MaterialDataBuffer.hpp"
#include "engine/resource/ResourceManager.hpp"
#include "engine/resource/ResourceHandle.hpp"

class RenderingSystem : public System {
    ResourceManager* resourceManager;
    GameObjectManager& gameObjectManager;
    Window& window;
    EventManager& eventManager;
    MaterialDataBuffer& globalMaterialBuffer;

    ClusteredRenderer* renderer;
    GBuffer* gBuffer;
    Camera* camera = nullptr;

    ResourceHandle<Shader> lightingShaderHandle;

    std::vector<GameObject*> lightCache, objectsCache;

    uint drawMode = 0;

    uint quadVAO = 0, quadVBO;
    void renderQuad();
    void onWindowResize(WindowResizeEvent& event);

    float time = 0;
    int currentFrame = 0;
public:
    RenderingSystem(
        ResourceManager& resourceManager_,
        GameObjectManager& gameObjectManager_,
        EventManager& eventManager_,
        Window& window_,
        MaterialDataBuffer& globalMaterialBuffer_
    );
    ~RenderingSystem();

    RenderingSystem(const RenderingSystem&) = delete;
    RenderingSystem& operator=(const RenderingSystem&) = delete;
    RenderingSystem(RenderingSystem&&) noexcept = delete;
    RenderingSystem& operator=(RenderingSystem&&) noexcept = delete;

    void update() override;
    void render(float deltaTime);
    void bindCamera(Camera* camera);
    void setDrawMode(uint newDrawMode);
    void updateCache();
};

#endif // ENGINE_GRAPHICS_RENDERINGSYSTEM_H_