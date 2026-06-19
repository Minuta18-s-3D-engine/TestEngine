#ifndef ENGINE_CORE_APPLICATION_HPP_
#define ENGINE_CORE_APPLICATION_HPP_

#include "CommandLineArgs.hpp"
#include "engine/project/VirtualPath.hpp"
#include "engine/debug/logging/Logging.hpp"
#include "engine/events/EventManager.hpp"
#include "engine/window/Window.hpp"
#include "engine/materials/Material.hpp"
#include "engine/graphics/RenderingSystem.hpp"
#include "engine/project/Project.hpp"
#include "engine/player/Player.hpp"

class Application {
    bool isRunning = true;
    Logger applicationLogger;
    CommandLineArgs args;

    std::unique_ptr<EventManager> eventManager;
    std::unique_ptr<Window> window;
    std::unique_ptr<MaterialDataBuffer> globalMaterialBuffer;
    std::unique_ptr<RenderingSystem> renderingSystem;
    std::unique_ptr<Project> project;

    void initEngineSystems();
    void destructEngineSystems();

    void loadProject(const VirtualPath& projectPath);

    // TODO: move this somewhere
    std::unique_ptr<Player> player;
    bool isInGame = true;

    void createTestScene();
    void processGameInput(float deltaTime);
public:
    Application(const CommandLineArgs& _args);
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void run();
};

#endif // ENGINE_CORE_APPLICATION_HPP_
