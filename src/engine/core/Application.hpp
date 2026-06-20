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
#include "engine/project/ProjectLoader.hpp"
#include "engine/assets/utils/MeshGen.hpp"
#include "engine/assets/utils/Files.hpp"
#include "engine/assets/coders/images/PngCoder.hpp"
#include "engine/assets/coders/images/JpgCoder.hpp"
#include "engine/materials/preprocessor/Preprocessor.hpp"
#include "engine/materials/templateGenerators/ShaderCodeGenerator.hpp"
#include "engine/materials/MaterialBuilder.hpp"
#include "engine/models/ModelLoader.hpp"

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
    Application(const CommandLineArgs& _args);
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void run();
};

#endif // ENGINE_CORE_APPLICATION_HPP_
