#include "Application.hpp"

namespace {

void createRect(
    glm::vec3 pos, glm::vec3 scale, glm::vec2 textureScale, 
    std::string baseMaterialKey,
    std::string diffuseTexKey,
    std::string specularTexKey,
    AssetManager& manager,
    GameObjectManager& objectManager,
    MaterialDataBuffer& buffer
) {
    const Material& baseMaterial = manager.require<Material>(baseMaterialKey);
    auto matInstance = std::make_shared<MaterialInstance>(
        baseMaterial.getName() + "Instance",
        baseMaterial,
        buffer
    );

    matInstance->setSampler(
        "diffuseMap", manager.getShared<Texture>(diffuseTexKey));
    matInstance->setSampler(
        "specularMap", manager.getShared<Texture>(specularTexKey));

    std::shared_ptr<Mesh> cubeMesh = generateCubeMesh(
        scale, textureScale, matInstance
    );

    std::vector<std::shared_ptr<Mesh>> cubeMeshArray;
    cubeMeshArray.push_back(cubeMesh);
    std::unique_ptr<Model> cubeModel = std::make_unique<Model>(cubeMeshArray);
    cubeModel->material = manager.getShared<Material>("materials/standardMaterial");
    
    uuids::uuid modelId = uuids::uuid_system_generator{}();
    std::string modelName = uuids::to_string(modelId);

    std::unique_ptr<GameObject> cubeObject = GameObject::createGameObject();
    auto transformComponent = cubeObject->getComponent<Transform>();
    transformComponent->position = pos;
    transformComponent->scale = scale;
    auto behaviorComponent = cubeObject->getComponent<Behavior>();
    behaviorComponent->type = BehaviorType::STATIC;
    auto modelComponent = std::make_unique<ModelComponent>();
    modelComponent->managerId = modelName;
    cubeObject->addComponent<ModelComponent>(modelComponent);

    objectManager.addObject(cubeObject);
    manager.set<Model>(std::move(cubeModel), modelName);
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

void loadTexture(
    const VirtualPath& path, std::string key, AssetManager& manager
) {
    size_t len = 0;
    auto texture_content = read_bytes(path.resolve(), len);
    std::shared_ptr<Texture> texture = nullptr;
    ImageType imgFormat = getImageType(texture_content.get(), len);
    
    if (imgFormat == ImageType::PNG) {
        texture = PngCoder::load_texture(texture_content.get(), len, key);
    } else if (imgFormat == ImageType::JPG) {
        texture = JpgCoder::load_texture(texture_content.get(), len, key);
    } else {
        throw std::runtime_error("Unsupported image type for texture: " + key);
    }

    manager.set<Texture>(texture, key);
}

enum class ShaderPreprocessingType {
    VERTEX,
    FRAGMENT,
    COMPUTATIONAL,
    OTHER
};

std::string processShaderSource(
    const VirtualPath& path, const Material& baseMaterial, 
    ShaderPreprocessingType type, Project& proj
) {
    Preprocessor preprocessor(proj.getFilesystem());

    std::string source;
    try {
        auto result = preprocessor.preprocess(path);

        source = result.first;
        if (!result.second.isEmpty()) {
            result.second.dumpToLogs();
        }
    } catch (ShaderDiagnostic::preprocessor_error& e) {
        e.getDiagnostic().dumpToLogs();
    }

    std::string callingFunc = "userFunc";
    if (type == ShaderPreprocessingType::VERTEX)
        callingFunc = "vertex";
    else if (type == ShaderPreprocessingType::FRAGMENT)
        callingFunc = "fragment";
    else if (type == ShaderPreprocessingType::COMPUTATIONAL)
        callingFunc = "comp";

    ShaderCodeGenerator generator;
    std::string generatedSource = generator.generateShader(
        baseMaterial, source, callingFunc
    );

    return generatedSource;
} 

Shader compileShader(
    const VirtualPath& vertex, const VirtualPath& frag,
    const Material& baseMaterial, Project& proj
) {
    std::string vertexSource = processShaderSource(
        vertex, baseMaterial, ShaderPreprocessingType::VERTEX, proj
    );

    std::string fragmentSource = processShaderSource(
        frag, baseMaterial, ShaderPreprocessingType::FRAGMENT, proj
    );

    return Shader(vertexSource, fragmentSource);
}

ComputeShader compileComputeShader(
    const VirtualPath& sourcePath, 
    Project& proj
) {
    Preprocessor preprocessor(proj.getFilesystem());

    std::string source;
    try {
        auto result = preprocessor.preprocess(sourcePath);

        source = result.first;
        if (!result.second.isEmpty()) {
            result.second.dumpToLogs();
        }
    } catch (ShaderDiagnostic::preprocessor_error& e) {
        e.getDiagnostic().dumpToLogs();
    }

    ShaderCodeGenerator generator;
    std::string generatedSource = generator.generateCompShader(
        source, "compute"
    );

    return ComputeShader(generatedSource);
}

}

Application::Application(const CommandLineArgs& _args)
  : args(_args),
    applicationLogger(Logging::createLogger("engine.application")) {
    initEngineSystems();

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
    applicationLogger.info(
        "Loading project from {}...", projectPath
    );

    std::filesystem::path projectFolder(args.get(cmdProjectKey));
    ProjectLoader projectLoader;
    project = projectLoader.loadProject(projectFolder, "./core");

    std::string mainSceneName = "MainScene";
    project->createEmptyScene(mainSceneName);
    project->setActiveScene(mainSceneName);
}

void Application::setupPlayerCamera() {
    player = std::make_unique<Player>(glm::vec3(0.0f, 2.0f, -1.0f));
    player->setPos(glm::vec3(37.153, 4.260, 3.347));
    player->getCamera()->setRotation(198.795, 4.165);
}

void Application::loadTextures() {
    AssetManager& assetManager = project->getAssetManager();
    VirtualPath path = "fs://assets/textures";

    for (const auto & entry : fs::directory_iterator(path.resolve())) {
        fs::path p = entry.path();
        std::string stem = p.stem().string();
        std::string ending = "Specular";

        loadTexture(
            "fs://assets/textures/" + p.filename().string(), 
            "textures/" + stem, 
            assetManager
        );
    }

    loadTexture(
        "core://assets/textures/missing.png",
        "textures/missing",
        assetManager 
    );
}

void Application::compileShadersAndMaterials() {
    AssetManager& assetManager = project->getAssetManager();

    MaterialGraphicsConfig standardMaterialConfig;

    std::shared_ptr<Material> standardMaterial = std::make_shared<Material>(
        MaterialBuilder(
            "StandardMaterial", standardMaterialConfig, assetManager
        )
        .addSampler("diffuseMap")
        .addSampler("specularMap")
        .finalize(*globalMaterialBuffer));
    assetManager.set<Material>(standardMaterial, "materials/standardMaterial");

    std::shared_ptr<Material> prototypeGrid = std::make_shared<Material>(
        MaterialBuilder(
            "PrototypeGrid", MaterialGraphicsConfig(), assetManager
        )
        .addProperty<glm::vec3>("baseColor", glm::vec3(0.8, 0.8, 0.8))
        .addProperty<float>("tilingScale", 1.0f)
        .finalize(*globalMaterialBuffer)
    );
    assetManager.set<Material>(prototypeGrid, "materials/prototypeGrid");

    auto& prototypeGridMaterial = assetManager.require<Material>(
        "materials/prototypeGrid"
    );
    Shader prototypeShader = compileShader(
        "fs://assets/shaders/julia/julia.vert.glsl",
        "fs://assets/shaders/julia/julia.frag.glsl",
        prototypeGridMaterial,
        *project
    );
    std::shared_ptr<Shader> prototypeShaderPtr = std::make_shared<Shader>(
        std::move(prototypeShader)
    );
    assetManager.set<Shader>(
        prototypeShaderPtr, "shaders/prototypeGridShader"
    );

    prototypeGridMaterial.bindShader(
        assetManager.getShared<Shader>("shaders/prototypeGridShader")
    );

    Shader geomShader = compileShader(
        "core://assets/shaders/geom.vert.glsl",
        "core://assets/shaders/geom.frag.glsl",
        assetManager.require<Material>("materials/standardMaterial"),
        *project
    );
    
    Shader lightingShader = compileShader(
        "core://assets/shaders/light.vert.glsl",
        "core://assets/shaders/light.frag.glsl",
        assetManager.require<Material>("materials/standardMaterial"),
        *project
    );
    std::shared_ptr<Shader> geomShaderPtr = std::make_shared<Shader>(
        std::move(geomShader));

    assetManager.require<Material>("materials/standardMaterial")
        .bindShader(geomShaderPtr);

    assetManager.set<Shader>(geomShaderPtr, "shaders/geomShader");
    assetManager.set<Shader>(std::make_shared<Shader>(
        std::move(lightingShader)), "shaders/lightingShader"
    );

    ComputeShader buildClustersShader = compileComputeShader(
        "core://assets/shaders/buildClusters.comp.glsl", 
        *project
    ); 
    ComputeShader lightCullingShader = compileComputeShader(
        "core://assets/shaders/lightCulling.comp.glsl",
        *project
    );

    assetManager.set<ComputeShader>(std::make_shared<ComputeShader>(
        std::move(buildClustersShader)), "shaders/buildClusters");
    assetManager.set<ComputeShader>(std::make_shared<ComputeShader>(
        std::move(lightCullingShader)), "shaders/lightCulling");
}

void Application::spawnSceneObjects() {
    AssetManager& assetManager = project->getAssetManager();
    GameObjectManager& objectManager = project->getActiveScene()
        .getGameObjectManager();

    auto prototypeGrid = assetManager.getShared<Material>(
        "materials/prototypeGrid"
    );

    auto matInstance = std::make_shared<MaterialInstance>(
        prototypeGrid->getName() + "Instance",
        assetManager.require<Material>("materials/prototypeGrid"),
        *globalMaterialBuffer
    );
    matInstance->setProperty("baseColor", glm::vec3(0.4, 0.8, 0.4));

    {
        const glm::vec3 scale(2.0f, 2.0f, 2.0f);
        const glm::vec2 textureScale(1.0f, 1.0f);
        const glm::vec3 pos(10.0f, 3.0f, 2.0f);

        std::shared_ptr<Mesh> cubeMesh = generateCubeMesh(
            scale, textureScale, matInstance
        );

        std::vector<std::shared_ptr<Mesh>> cubeMeshArray;
        cubeMeshArray.push_back(cubeMesh);
        std::unique_ptr<Model> cubeModel = std::make_unique<Model>(
            cubeMeshArray
        );
        cubeModel->material = prototypeGrid;
        
        std::string modelName = "checkerCube";

        std::unique_ptr<GameObject> cubeObject = 
            GameObject::createGameObject();
        auto transformComponent = cubeObject->getComponent<Transform>();
        transformComponent->position = pos;
        transformComponent->scale = scale;
        auto behaviorComponent = cubeObject->getComponent<Behavior>();
        behaviorComponent->type = BehaviorType::STATIC;
        auto modelComponent = std::make_unique<ModelComponent>();
        modelComponent->managerId = modelName;
        cubeObject->addComponent<ModelComponent>(modelComponent);

        objectManager.addObject(cubeObject);
        assetManager.set<Model>(std::move(cubeModel), modelName);
    }

    createRect(
        glm::vec3(2.0, 2.0, 5.0), glm::vec3(1.0, 1.0, 1.0), 
        glm::vec2(1.0, 1.0), "materials/standardMaterial", 
        "textures/container", "textures/containerSpecular",
        assetManager, objectManager, *globalMaterialBuffer
    );

    ModelLoader modelLoader;

    {
        auto baseMaterial = assetManager.getShared<Material>(
            "materials/standardMaterial");
        auto sponzaModel = modelLoader.loadModel(
            "fs://assets/models/sponza_low_res.glb", baseMaterial, assetManager
        );
        auto modelName = "sponza_model";

        std::unique_ptr<GameObject> sponzaObject =
            GameObject::createGameObject();
        auto transformComponent = sponzaObject->getComponent<Transform>();
        transformComponent->position = glm::vec3(0.0f, 1.0f, 0.0f);
        transformComponent->scale = glm::vec3(1.0f, 1.0f, 1.0f);
        auto behaviorComponent = sponzaObject->getComponent<Behavior>();
        behaviorComponent->type = BehaviorType::STATIC;
        auto modelComponent = std::make_unique<ModelComponent>();
        modelComponent->managerId = modelName;
        sponzaObject->addComponent<ModelComponent>(modelComponent);

        objectManager.addObject(sponzaObject);
        assetManager.set<Model>(std::move(sponzaModel), modelName);
    }
}

void Application::loadLights() {
    std::ifstream lightsFile(VirtualPath("fs://lights.txt").resolve());
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
    applicationLogger.info("Createing test scene...");

    setupPlayerCamera();
    loadTextures();
    compileShadersAndMaterials();
    spawnSceneObjects();
    loadLights();

    renderingSystem = std::make_unique<RenderingSystem>(
        project->getAssetManager(),
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
    if (input.isKeyPressed(GLFW_KEY_W)) 
        newPos += (frontVec * delta * player->getSpeed());
    if (input.isKeyPressed(GLFW_KEY_S))
        newPos -= (frontVec * delta * player->getSpeed());
    if (input.isKeyPressed(GLFW_KEY_A))
        newPos -= (rightVec * delta * player->getSpeed());
    if (input.isKeyPressed(GLFW_KEY_D))
        newPos += (rightVec * delta * player->getSpeed());
    if (input.isKeyPressed(GLFW_KEY_SPACE))
        newPos.y += delta * player->getSpeed();
    if (input.isKeyPressed(GLFW_KEY_LEFT_SHIFT))
        newPos.y -= delta * player->getSpeed();
    if (input.isKeyJustPressed(GLFW_KEY_P)) {
        std::cout << "Player position: ";
        std::cout << player->getCamera()->pos.x << " " 
                    << player->getCamera()->pos.y << " " 
                    << player->getCamera()->pos.z << std::endl;
        std::cout << "Player camera angle: ";
        std::cout << player->getCamera()->getYaw() << " "
                    << player->getCamera()->getPitch() << std::endl;
    }

    player->setPos(newPos);
    player->update(delta);

    if (input.isKeyPressed(GLFW_KEY_F1)) 
        renderingSystem->setDrawMode(0);
    if (input.isKeyPressed(GLFW_KEY_F2)) 
        renderingSystem->setDrawMode(1);
    if (input.isKeyPressed(GLFW_KEY_F3)) 
        renderingSystem->setDrawMode(2);
}

void Application::run() {
    renderingSystem->updateCache();
    
    float lastFrame = glfwGetTime();
    float lastFPSDisplay = lastFrame;
    int framesCount = 0;

    while (!window->isShouldClose()) {
        float currentFrame = glfwGetTime();
        float delta = currentFrame - lastFrame;
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
