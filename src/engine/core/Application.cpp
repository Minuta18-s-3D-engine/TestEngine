#include "Application.hpp"

#include "engine/resource/importers/TextureImporter.hpp"
#include "engine/materials/preprocessor/Preprocessor.hpp"
#include "engine/materials/templateGenerators/ShaderCodeGenerator.hpp"
#include "engine/project/ProjectLoader.hpp"
#include "engine/materials/MaterialBuilder.hpp"
#include "engine/models/ModelLoader.hpp"
#include "engine/events/EventManager.hpp"
#include "engine/window/Window.hpp"
#include "engine/graphics/RenderingSystem.hpp"
#include "engine/project/Project.hpp"
#include "engine/player/Player.hpp"
#include "engine/resource/ResourceManager.hpp"
#include "engine/resource/serializers/JsonSerializer.hpp"

namespace {

void createRect(
    const glm::vec3 pos,
    const glm::vec3 scale,
    const glm::vec2 textureScale,
    ResourceHandle<Material> baseMaterialHandle,
    ResourceHandle<Texture> diffuseTexHandle,
    ResourceHandle<Texture> specularTexHandle,
    ResourceManager& resManager,
    GameObjectManager& objectManager,
    MaterialDataBuffer& buffer
) {
    const Material& baseMaterial = resManager.require<Material>(baseMaterialHandle);
    const auto matInstance = std::make_shared<MaterialInstance>(
        baseMaterial,
        buffer,
        resManager
    );

    matInstance->setSampler("diffuseMap", diffuseTexHandle);
    matInstance->setSampler("specularMap", specularTexHandle);

    auto matInstanceHandle = resManager.addManually(*matInstance);

    const ResourceHandle<Mesh> cubeMeshHandle = generateCubeMesh(
        scale, textureScale, matInstanceHandle, resManager);

    std::vector<ResourceHandle<Mesh>> cubeMeshArray{cubeMeshHandle};

    const auto cubeModel = std::make_unique<Model>(resManager, cubeMeshArray);
    cubeModel->material = baseMaterialHandle;

    const uuids::uuid modelId = uuids::uuid_system_generator{}();
    VirtualPath modelPath("memory://models/" + uuids::to_string(modelId));
    const ResourceHandle<Model> modelHandle =
        resManager.addManually<Model>(*cubeModel);

    std::unique_ptr<GameObject> cubeObject = GameObject::createGameObject();
    const auto transformComponent = cubeObject->getComponent<Transform>();
    transformComponent->position = pos;
    transformComponent->scale = scale;

    const auto behaviorComponent = cubeObject->getComponent<Behavior>();
    behaviorComponent->type = BehaviorType::STATIC;

    auto modelComponent = std::make_unique<ModelComponent>(modelHandle);
    cubeObject->addComponent<ModelComponent>(modelComponent);

    objectManager.addObject(cubeObject);
}

void createPointLight(
    glm::vec3 pos, glm::vec3 color, float linear, float quadratic,
    GameObjectManager& objectManager
) {
    std::unique_ptr<GameObject> lightObject = GameObject::createGameObject();
    auto transformComponent = lightObject->getComponent<Transform>();
    transformComponent->position = pos;
    auto behaviorComponent = lightObject->getComponent<Behavior>();
    behaviorComponent->type = BehaviorType::STATIC;
    auto pointLightComponent = std::make_unique<PointLight>();
    pointLightComponent->color = color;
    pointLightComponent->linear = linear;
    pointLightComponent->quadratic = quadratic;
    lightObject->addComponent<PointLight>(pointLightComponent);

    objectManager.addObject(lightObject);
}

} // namespace

Application::Application(const CommandLineArgs& args_)
  : args(args_),
    applicationLogger(Logging::createLogger("engine.application")) {
    initEngineSystems();

    serializer = std::make_unique<JsonSerializer>();

    if (!args.has(cmdProjectKey)) {
        applicationLogger.fatal("No project set. Use --project <path>");
        throw std::runtime_error("No project set");
    }

    loadProject(args.get(cmdProjectKey));
    createTestScene();
}

Application::~Application() {
    applicationLogger.info("Engine shut down");
}

void Application::initEngineSystems() {
    applicationLogger.info("Beginning system initialization...");

    eventManager = std::make_unique<EventManager>();
    window = std::make_unique<Window>(*eventManager);
    globalMaterialBuffer = std::make_unique<MaterialDataBuffer>();
}

void Application::loadProject(const std::string& projectPath) {
    applicationLogger.info("Loading project from {}...", projectPath);

    std::filesystem::path projectFolder(args.get(cmdProjectKey));
    ProjectLoader projectLoader;
    project = projectLoader.loadProject(projectFolder, "./core");

    resourceManager = std::make_unique<ResourceManager>();

    auto textureImporter = std::make_unique<TextureImporter>(
        *serializer
    );
    resourceManager->registerImporter<Texture>(std::move(textureImporter));

    const std::string mainSceneName = "MainScene";
    project->createEmptyScene(mainSceneName);
    project->setActiveScene(mainSceneName);
}

void Application::setupPlayerCamera() {
    player = std::make_unique<Player>(glm::vec3(0.0f, 2.0f, -1.0f));
    player->setPos(glm::vec3(37.153, 4.260, 3.347));
    player->getCamera()->setRotation(198.795, 4.165);
}

void Application::loadTextures() {
    VirtualPath path = "fs://assets/textures";

    for (const auto& entry : fs::directory_iterator(project->resolve(path.resolve()))) {
        fs::path p = entry.path();
        if (p.extension() == ".meta") continue;

        VirtualPath texturePath("fs://assets/textures/" + p.filename().string());
        resourceManager->load<Texture>(texturePath);
    }

    resourceManager->load<Texture>(VirtualPath("core://assets/textures/missing.png"));
}

std::string Application::generateShaderSource(
    const VirtualPath& sourcePath,
    const ShaderLayout& shaderLayout,
    const std::string& callFunc,
    bool generateUnpack
) {
    const std::string source = project->getFilesystem().readFile(sourcePath);

    const ShaderCodeGenerator generator(
        *resourceManager,
        ShaderCodeGenerator::FormattingOptions()
    );
    std::string fullSource = generator.generateShader(
        shaderLayout, source, callFunc, generateUnpack
    );

    return fullSource;
}

void Application::compileShadersAndMaterials() {
    {
        ShaderLayout geometryPassShaderLayout;
        geometryPassShaderLayout.addSampler("diffuseMap", ShaderLayout::SamplerType::Texture2D);
        geometryPassShaderLayout.addSampler("specularMap", ShaderLayout::SamplerType::Texture2D);
        geometryPassShaderLayout.finalize();
        std::string vertexSource = generateShaderSource(
            "fs://assets/shaders/geom.vert.glsl", geometryPassShaderLayout,
            "vertex");
        std::string fragmentSource = generateShaderSource(
            "fs://assets/shaders/geom.frag.glsl", geometryPassShaderLayout,
            "fragment");
        Shader geometryPassShader({
            .vertex = vertexSource,
            .fragment = fragmentSource
        }, std::move(geometryPassShaderLayout));
        ResourceHandle<Shader> geometryPassShaderHandle = resourceManager->addManually<Shader>(
            "fs://assets/shaders/geom.vert.glsl", std::move(geometryPassShader));

        Material defaultTexturedMaterial = MaterialBuilder(
            geometryPassShaderHandle, *resourceManager, MaterialGraphicsConfig(), *globalMaterialBuffer
        ).finalize();
        ResourceHandle<Material> defaultTexturedMaterialHandle = resourceManager->addManually<Material>(
            "fs://materials/prototypeGrid", std::move(defaultTexturedMaterial)
        );
    }

    {
        ShaderLayout lightingPassShaderLayout;
        lightingPassShaderLayout.finalize();
        std::string vertexSource = generateShaderSource(
            "fs://assets/shaders/light.vert.glsl", lightingPassShaderLayout,
            "vertex");
        std::string fragmentSource = generateShaderSource(
            "fs://assets/shaders/light.frag.glsl", lightingPassShaderLayout,
            "fragment");
        Shader lightingPassShader({
            .vertex = vertexSource,
            .fragment = fragmentSource
        }, std::move(lightingPassShaderLayout));
        ResourceHandle<Shader> lightingPassShaderHandle = resourceManager->addManually<Shader>(
            "fs://assets/shaders/light.vert.glsl", std::move(lightingPassShader));
    }

    {
        ShaderLayout prototypeShaderLayout;
        prototypeShaderLayout.addProperty("baseColor", ShaderLayout::PropertyType::Vec3);
        prototypeShaderLayout.addProperty("tilingScale", ShaderLayout::PropertyType::Float);
        prototypeShaderLayout.finalize();
        std::string vertexSource = generateShaderSource(
            "fs://assets/shaders/julia/julia.vert.glsl", prototypeShaderLayout,
            "vertex");
        std::string fragmentSource = generateShaderSource(
            "fs://assets/shader/julia/julia.frag.glsl", prototypeShaderLayout,
            "fragment");
        Shader prototypeShader({
            .vertex = vertexSource,
            .fragment = fragmentSource
        }, std::move(prototypeShaderLayout));
        ResourceHandle<Shader> prototypeShaderHandle = resourceManager->addManually<Shader>(
            "fs://assets/shaders/julia/julia.vert.glsl", std::move(prototypeShader)); 
        
        Material prototypeMaterial = MaterialBuilder(
            prototypeShaderHandle, *resourceManager, MaterialGraphicsConfig(), *globalMaterialBuffer
        ).setProperty("baseColor", glm::vec3(0.8, 0.8, 0.8))
            .setProperty("tilingScale", 1.0f)
            .finalize();
        ResourceHandle<Material> prototypeMaterialHandle = resourceManager->addManually<Material>(
            "fs://materials/prototypeMaterial", std::move(prototypeMaterial)
        );
    }

    {
        ShaderLayout buildClustersShaderLayout;
        buildClustersShaderLayout.finalize();
        std::string computeSource = generateShaderSource(
            "fs://assets/shaders/buildClusters.comp.glsl", buildClustersShaderLayout,
            "compute");
        Shader buildClustersShader({
            .compute = computeSource
        }, std::move(buildClustersShaderLayout));
        ResourceHandle<Shader> buildClustersShaderHandle = resourceManager->addManually<Shader>(
            "fs://assets/shaders/buildClusters.comp.glsl", std::move(buildClustersShader));
    }

    {
        ShaderLayout lightCullingShaderLayout;
        lightCullingShaderLayout.finalize();
        std::string computeSource = generateShaderSource(
            "fs://assets/shaders/lightCulling.comp.glsl", lightCullingShaderLayout,
            "compute");
        Shader lightCullingShader({
            .compute = computeSource
        }, std::move(lightCullingShaderLayout));
        ResourceHandle<Shader> lightCullingShaderHandle = resourceManager->addManually<Shader>(
            "fs://assets/shaders/lightCulling.comp.glsl", std::move(lightCullingShader));
    }
}

void Application::spawnSceneObjects() {
    GameObjectManager& objectManager = project->getActiveScene().getGameObjectManager();

    ResourceHandle<Material> protoGridMatHandle = resourceManager->getByPath<Material>(
        "fs://materials/prototypeGrid"
    );
    ResourceHandle<Material> stdMatHandle = resourceManager->getByPath<Material>(
        "core://materials/standardMaterial"
    );

    {
        auto matInstance = std::make_shared<MaterialInstance>(
            resourceManager->require(protoGridMatHandle),
            *globalMaterialBuffer,
            *resourceManager
        );
        matInstance->setProperty("baseColor", glm::vec3(0.4, 0.8, 0.4));
        auto matInstanceHandle = resourceManager->addManually<MaterialInstance>(*matInstance);

        const glm::vec3 scale(2.0f, 2.0f, 2.0f);
        const glm::vec2 textureScale(1.0f, 1.0f);
        const glm::vec3 pos(10.0f, 3.0f, 2.0f);

        ResourceHandle<Mesh> cubeMeshHandle = generateCubeMesh(
            scale, textureScale, matInstanceHandle, *resourceManager
        );
        Model cubeModel(
            *resourceManager,
            std::vector<ResourceHandle<Mesh>>{cubeMeshHandle}
        );
        cubeModel.material = protoGridMatHandle;

        ResourceHandle<Model> checkerCubeHandle = resourceManager->addManually<Model>(
            "memory://models/checkerCube",
            std::move(cubeModel)
        );

        std::unique_ptr<GameObject> cubeObject = GameObject::createGameObject();
        cubeObject->getComponent<Transform>()->position = pos;
        cubeObject->getComponent<Transform>()->scale = scale;
        cubeObject->getComponent<Behavior>()->type = BehaviorType::STATIC;

        auto modelComponent = std::make_unique<ModelComponent>(checkerCubeHandle);
        cubeObject->addComponent<ModelComponent>(modelComponent);

        objectManager.addObject(cubeObject);
    }

    createRect(
        glm::vec3(2.0, 2.0, 5.0), glm::vec3(1.0, 1.0, 1.0), glm::vec2(1.0, 1.0),
        stdMatHandle,
        resourceManager->getByPath<Texture>(VirtualPath("fs://assets/textures/container.png")),
        resourceManager->getByPath<Texture>(VirtualPath("fs://assets/textures/containerSpecular.png")),
        *resourceManager, objectManager, *globalMaterialBuffer
    );

    // Sponza
    {
        ModelLoader modelLoader;
        auto sponzaModel = modelLoader.loadModel(
            "fs://assets/models/sponza_low_res.glb",
            stdMatHandle, *resourceManager
        );

        ResourceHandle<Model> sponzaHandle = resourceManager->addManually<Model>(
            VirtualPath("fs://assets/models/sponza_low_res.glb"),
            std::move(*sponzaModel)
        );

        std::unique_ptr<GameObject> sponzaObject = GameObject::createGameObject();
        sponzaObject->getComponent<Transform>()->position = glm::vec3(0.0f, 1.0f, 0.0f);
        sponzaObject->getComponent<Transform>()->scale = glm::vec3(1.0f, 1.0f, 1.0f);
        sponzaObject->getComponent<Behavior>()->type = BehaviorType::STATIC;

        auto modelComponent = std::make_unique<ModelComponent>(sponzaHandle);
        sponzaObject->addComponent<ModelComponent>(modelComponent);

        objectManager.addObject(sponzaObject);
    }
}

void Application::loadLights() {
    std::ifstream lightsFile(project->resolve("fs://lights.txt"));
    std::string line;
    while (getline(lightsFile, line)) {
        std::stringstream parseLine(line);
        float lightX, lightY, lightZ;
        parseLine >> lightX >> lightY >> lightZ;
        float lightR, lightG, lightB;
        parseLine >> lightR >> lightG >> lightB;

        createPointLight(
            glm::vec3(lightX, lightY, lightZ),
            glm::vec3(lightR, lightG, lightB),
            0.01, 0.02, project->getActiveScene().getGameObjectManager()
        );
    }
}

void Application::createTestScene() {
    applicationLogger.info("Creating test scene...");

    setupPlayerCamera();
    loadTextures();
    compileShadersAndMaterials();
    spawnSceneObjects();
    loadLights();

    renderingSystem = std::make_unique<RenderingSystem>(
        *resourceManager,
        project->getActiveScene().getGameObjectManager(),
        *eventManager,
        *window,
        *globalMaterialBuffer
    );
    renderingSystem->bindCamera(player->getCamera().get());
}

void Application::processGameInput(float delta) {
    const float MOUSE_SENSITIVITY = 0.1f;
    InputController& input = window->getInputController();

    if (input.isKeyJustPressed(GLFW_KEY_ESCAPE)) {
        window->setCursorInputMode(
            window->getCursorInputMode() == GLFW_CURSOR_NORMAL ?
            GLFW_CURSOR_DISABLED :
            GLFW_CURSOR_NORMAL
        );
        isInGame = !isInGame;
    }

    if (isInGame) {
        glm::vec2 cursor = input.getMouseMovement();
        player->getCamera()->processMouseMovement(
            cursor.x, -cursor.y, MOUSE_SENSITIVITY
        );
    }

    glm::vec3 frontVec = glm::normalize(player->getCamera()->front);
    glm::vec3 rightVec = glm::normalize(player->getCamera()->right);
    glm::vec3 newPos = player->getPos();
    if (input.isKeyPressed(GLFW_KEY_W)) newPos += (frontVec * delta * player->getSpeed());
    if (input.isKeyPressed(GLFW_KEY_S)) newPos -= (frontVec * delta * player->getSpeed());
    if (input.isKeyPressed(GLFW_KEY_A)) newPos -= (rightVec * delta * player->getSpeed());
    if (input.isKeyPressed(GLFW_KEY_D)) newPos += (rightVec * delta * player->getSpeed());
    if (input.isKeyPressed(GLFW_KEY_SPACE)) newPos.y += delta * player->getSpeed();
    if (input.isKeyPressed(GLFW_KEY_LEFT_SHIFT)) newPos.y -= delta * player->getSpeed();

    player->setPos(newPos);
    player->update(delta);

    if (input.isKeyPressed(GLFW_KEY_F1)) renderingSystem->setDrawMode(0);
    if (input.isKeyPressed(GLFW_KEY_F2)) renderingSystem->setDrawMode(1);
    if (input.isKeyPressed(GLFW_KEY_F3)) renderingSystem->setDrawMode(2);
}

void Application::run() {
    renderingSystem->updateCache();

    auto lastFrame = static_cast<float>(glfwGetTime());
    float lastFPSDisplay = lastFrame;
    int framesCount = 0;

    while (!window->isShouldClose()) {
        const auto currentFrame = static_cast<float>(glfwGetTime());
        const float delta = currentFrame - lastFrame;
        lastFrame = currentFrame;

        window->getInputController().updateEvents();
        eventManager->dispatchEvents();

        processGameInput(delta);

        framesCount++;
        if (currentFrame - lastFPSDisplay >= 1.0f) {
            window->setCaption(
                project->getName() + " (fps = " +
                std::to_string(framesCount) + ")"
            );
            framesCount = 0;
            lastFPSDisplay = currentFrame;
        }

        globalMaterialBuffer->sync();
        renderingSystem->render(delta);

        window->swapBuffers();
    }
}
