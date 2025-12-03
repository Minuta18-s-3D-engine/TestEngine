#ifndef ENGINE_GRAPHICS_RENDERINGSYSTEM_H_
#define ENGINE_GRAPHICS_RENDERINGSYSTEM_H_

#include "../gameobject/System.hpp"
#include "../gameobject/GameObject.hpp"
#include "../gameobject/GameObjectManager.hpp"
#include "../gameobject/components/Transform.hpp"
#include "../gameobject/components/Behavior.hpp"
#include "components/PointLight.hpp"

#include "../assets/AssetManager.hpp"
#include "../graphics/ClusteredRenderer.hpp"
#include "../graphics/GBuffer.hpp"
#include "../graphics/Camera.hpp"
#include "../window/Window.hpp"

#include <memory>
#include <vector>

class RenderingSystem : public System {
    AssetManager& assetManager;
    GameObjectManager& gameObjectManager;

    ClusteredRenderer* renderer;
    GBuffer* gBuffer;
    Camera* camera = nullptr;

    uint drawMode = 0;
public:
    RenderingSystem(
        AssetManager& _assetManager, 
        GameObjectManager& _gameObjectManager
    );
    ~RenderingSystem();

    void update() override;
    void render();
    void bindCamera(Camera* camera);
    void setDrawMode(uint newDrawMode);
};

#endif // ENGINE_GRAPHICS_RENDERINGSYSTEM_H_