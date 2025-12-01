#ifndef ENGINE_GRAPHICS_RENDERINGSYSTEM_H_
#define ENGINE_GRAPHICS_RENDERINGSYSTEM_H_

#include "../gameobject/System.hpp"

#include "../assets/AssetManager.hpp"
#include "../graphics/ClusteredRenderer.hpp"
#include "../graphics/GBuffer.hpp"
#include "../window/Window.hpp"

#include <memory>
#include <vector>

class RenderingSystem : public System {
    AssetManager& assetManager;

    ClusteredRenderer* renderer;
    GBuffer* gBuffer;

    uint drawMode = 0;
public:
    RenderingSystem(AssetManager& _assetManager);

    void update() override;
};

#endif // ENGINE_GRAPHICS_RENDERINGSYSTEM_H_