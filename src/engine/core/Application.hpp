#ifndef ENGINE_CORE_APPLICATION_HPP_
#define ENGINE_CORE_APPLICATION_HPP_

#include "CommandLineArgs.hpp"
#include "engine/project/VirtualPath.hpp"
#include "engine/debug/logging/Logging.hpp"

#define UUID_SYSTEM_GENERATOR
#include <uuid.h>

class EventManager;
class Window;
class MaterialDataBuffer;
class RenderingSystem;
class Project;
class ResourceManager;
class JsonSerializer;
class Player;

class Application {
    const std::string cmdProjectKey = "--project";

    bool isRunning = true;
    Logger applicationLogger;
    CommandLineArgs args;

    std::unique_ptr<EventManager> eventManager;
    std::unique_ptr<Window> window;
    std::unique_ptr<MaterialDataBuffer> globalMaterialBuffer;
    std::unique_ptr<RenderingSystem> renderingSystem;
    std::unique_ptr<Project> project;
    std::unique_ptr<ResourceManager> resourceManager;
    std::unique_ptr<JsonSerializer> serializer;

    void initEngineSystems();

    void loadProject(const std::string& projectPath);

    // TODO: move this somewhere
    std::unique_ptr<Player> player;
    bool isInGame = true;

    void createTestScene();
    void setupPlayerCamera();
    void loadTextures();
    void compileShadersAndMaterials();
    void spawnSceneObjects();
    void loadLights();
    void processGameInput(float deltaTime);
public:
    explicit Application(const CommandLineArgs& args_);
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void run();
};

#endif // ENGINE_CORE_APPLICATION_HPP_
