#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define UUID_SYSTEM_GENERATOR

#include <uuid.h>

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <memory>
#include <filesystem>
#include <sstream>
#include <chrono>
#include <unordered_map>

#include "engine/graphics/Shader.hpp"
#include "engine/graphics/ComputeShader.hpp"
#include "engine/window/Window.hpp"
#include "engine/window/InputController.hpp"
#include "engine/assets/coders/images/PngCoder.hpp"
#include "engine/assets/utils/Files.hpp"
#include "engine/assets/AssetManager.hpp"
#include "engine/graphics/Texture.hpp"
#include "engine/player/Player.hpp"
#include "engine/models/Mesh.hpp"
#include "engine/models/Model.hpp"
#include "engine/models/ModelComponent.hpp"
#include "engine/gameObject/GameObjectManager.hpp"
#include "engine/gameObject/GameObject.hpp"
#include "engine/graphics/components/PointLight.hpp"
#include "engine/graphics/RenderingSystem.hpp"
#include "engine/models/ModelLoader.hpp"
#include "engine/assets/utils/MeshGen.hpp"
#include "engine/project/Project.hpp"
#include "engine/project/ProjectLoader.hpp"
#include "engine/materials/preprocessor/Preprocessor.hpp"

#include "engine/materials/Material.hpp"
#include "engine/materials/MaterialBuilder.hpp"
#include "engine/materials/MaterialDataBuffer.hpp"
#include "engine/materials/templateGenerators/ShaderCodeGenerator.hpp"

namespace fs = std::filesystem;

const float MOUSE_SENSITIVITY = 0.10;

// temporary solution
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

std::unordered_map<std::string, std::string> parseArguments(
    int argc, char* argv[]
) { 
    std::unordered_map<std::string, std::string> result;
    if (argc <= 1) return result;
    std::string currentArg = argv[0];
    for (int i = 1; i < argc; ++i) {
        bool option1 = (currentArg.compare(0, 2, "--") == 0);
        bool option2 = !(std::string(argv[i]).compare(0, 2, "--") == 0);
        if (option1 && option2) {
            result[currentArg] = argv[i];
        }
        currentArg = std::string(argv[i]);
    }
    return result;
}

int main(int argc, char* argv[]) {
    std::unordered_map<std::string, std::string> args = 
        parseArguments(argc, argv);

    if (!args.contains("--project")) {
        std::cerr << "No project set." << std::endl;
        return -1;
    }

    std::unique_ptr<Project> projectPtr;
    try {
        std::filesystem::path projectFolder(args["--project"]);
        ProjectLoader projectLoader;
        projectPtr = projectLoader.loadProject(
            projectFolder, "./core");
    } catch (const exc::file_not_found& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    } catch (const exc::validation_error& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    EventManager eventManager;
    Window win(eventManager);

    Project& project = *projectPtr;
    AssetManager& assetManager = project.getAssetManager();

    std::string mainSceneName = "MainScene";
    project.createEmptyScene(mainSceneName);
    Scene& scene = project.getScene(mainSceneName);
    GameObjectManager& objectManager = scene.getGameObjectManager();

    InputController& inputController = win.getInputController();

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

    MaterialDataBuffer globalMaterialBuffer;

    {
        MaterialGraphicsConfig standardMaterialConfig;

        std::shared_ptr<Material> standardMaterial = std::make_shared<Material>(MaterialBuilder(
            "StandardMaterial", standardMaterialConfig, assetManager
        ).addSampler("diffuseMap")
            .addSampler("specularMap")
            .finalize(globalMaterialBuffer));

        assetManager.set<Material>(standardMaterial, "materials/standardMaterial");
    }

    {
        std::shared_ptr<Material> prototypeGrid = std::make_shared<Material>(
            MaterialBuilder(
                "PrototypeGrid", MaterialGraphicsConfig(), assetManager
            )
            .addProperty<glm::vec3>("baseColor", glm::vec3(0.8, 0.8, 0.8))
            .addProperty<float>("tilingScale", 1.0f)
            .finalize(globalMaterialBuffer)
        );
          
        assetManager.set<Material>(prototypeGrid, "materials/prototypeGrid");

        auto& prototypeGridMaterial = assetManager.require<Material>(
            "materials/prototypeGrid"
        );

        Shader prototypeShader = compileShader(
            "fs://assets/shaders/proto1/proto1.vert.glsl",
            "fs://assets/shaders/proto1/proto1.frag.glsl",
            prototypeGridMaterial,
            project
        );
        std::shared_ptr<Shader> prototypeShaderPtr = std::make_shared<Shader>(
            std::move(prototypeShader)
        );
        assetManager.set<Shader>(
            prototypeShaderPtr, "shaders/prototypeGridShader"
        );

        prototypeGridMaterial.bindShader(assetManager.getShared<Shader>("shaders/prototypeGridShader"));

        auto matInstance = std::make_shared<MaterialInstance>(
            prototypeGrid->getName() + "Instance",
            assetManager.require<Material>("materials/prototypeGrid"),
            globalMaterialBuffer
        );
        matInstance->setProperty("baseColor", glm::vec3(0.4, 0.8, 0.4));

        const glm::vec3 scale(2.0f, 2.0f, 2.0f);
        const glm::vec2 textureScale(1.0f, 1.0f);
        const glm::vec3 pos(10.0f, 3.0f, 2.0f);

        std::shared_ptr<Mesh> cubeMesh = generateCubeMesh(
            scale, textureScale, matInstance
        );

        std::vector<std::shared_ptr<Mesh>> cubeMeshArray;
        cubeMeshArray.push_back(cubeMesh);
        std::unique_ptr<Model> cubeModel = std::make_unique<Model>(cubeMeshArray);
        cubeModel->material = prototypeGrid;
        
        std::string modelName = "checkerCube";

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
        assetManager.set<Model>(std::move(cubeModel), modelName);
    }

    Shader geomShader = compileShader(
        "core://assets/shaders/geom.vert.glsl",
        "core://assets/shaders/geom.frag.glsl",
        assetManager.require<Material>("materials/standardMaterial"),
        project
    );
    
    Shader lightingShader = compileShader(
        "core://assets/shaders/light.vert.glsl",
        "core://assets/shaders/light.frag.glsl",
        assetManager.require<Material>("materials/standardMaterial"),
        project
    );
    std::shared_ptr<Shader> geomShaderPtr = std::make_shared<Shader>(std::move(geomShader));

    assetManager.require<Material>("materials/standardMaterial").bindShader(geomShaderPtr);

    assetManager.set<Shader>(geomShaderPtr, "shaders/geomShader");
    assetManager.set<Shader>(std::make_shared<Shader>(std::move(lightingShader)), "shaders/lightingShader");

    ComputeShader buildClustersShader = compileComputeShader(
        "core://assets/shaders/buildClusters.comp.glsl", 
        project
    ); 
    ComputeShader lightCullingShader = compileComputeShader(
        "core://assets/shaders/lightCulling.comp.glsl",
        project
    );

    assetManager.set<ComputeShader>(std::make_shared<ComputeShader>(std::move(buildClustersShader)), "shaders/buildClusters");
    assetManager.set<ComputeShader>(std::make_shared<ComputeShader>(std::move(lightCullingShader)), "shaders/lightCulling");

    RenderingSystem renderingSystem(
        assetManager,
        objectManager,
        eventManager,
        win,
        globalMaterialBuffer
    );

    Player player(glm::vec3(0.0f, 2.0f, -1.0f));
    player.setPos(glm::vec3(37.153, 4.26025, 3.34797));
    player.getCamera()->setRotation(198.795, 4.16543);
    renderingSystem.bindCamera(player.getCamera().get());

    createRect(
        glm::vec3(2.0, 2.0, 5.0), glm::vec3(1.0, 1.0, 1.0), 
        glm::vec2(1.0, 1.0), "materials/standardMaterial", "textures/container", "textures/containerSpecular",
        assetManager, objectManager, globalMaterialBuffer
    );

    ModelLoader modelLoader;
    
    {
        auto baseMaterial = assetManager.getShared<Material>("materials/standardMaterial");
        auto sponzaModel = modelLoader.loadModel(
            "fs://assets/models/sponza_low_res.glb", baseMaterial, assetManager
        );
        auto modelName = "sponza_model";

        std::unique_ptr<GameObject> sponzaObject = GameObject::createGameObject();
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
            0.01, 0.02, objectManager
        );
    }

    renderingSystem.updateCache();

    float lastFrame = 0.0f, currentFrame = 0.0f, deltaTime;
    float lastFPSDisplay = -10.0f;
    int framesCount = 0;
    bool isInGame = true;
    while (!win.isShouldClose()) {
        inputController.updateEvents();
        eventManager.dispatchEvents();

        if (isInGame) {
            glm::vec2 cursorMovement = inputController.getMouseMovement();
            player.getCamera()->processMouseMovement(
                cursorMovement.x, -cursorMovement.y, 
                MOUSE_SENSITIVITY
            );
        }

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        framesCount += 1;
        if (currentFrame - lastFPSDisplay >= 1.0) {
            win.setCaption(project.getName() + " (fps = " + std::to_string(framesCount) + ")");
            framesCount = 0;
            lastFPSDisplay = currentFrame;
        }

        glm::vec3 frontVec = glm::normalize(player.getCamera()->front); 
            // TODO: move this logic to player class
        glm::vec3 rightVec = glm::normalize(player.getCamera()->right);
        glm::vec3 newPos = player.getPos();
        if (inputController.isKeyPressed(GLFW_KEY_W)) 
            newPos += (frontVec * deltaTime * player.getSpeed());
        if (inputController.isKeyPressed(GLFW_KEY_S))
            newPos -= (frontVec * deltaTime * player.getSpeed());
        if (inputController.isKeyPressed(GLFW_KEY_A))
            newPos -= (rightVec * deltaTime * player.getSpeed());
        if (inputController.isKeyPressed(GLFW_KEY_D))
            newPos += (rightVec * deltaTime * player.getSpeed());
        if (inputController.isKeyPressed(GLFW_KEY_SPACE))
            newPos.y += deltaTime * player.getSpeed();
        if (inputController.isKeyPressed(GLFW_KEY_LEFT_SHIFT))
            newPos.y -= deltaTime * player.getSpeed();
        if (inputController.isKeyJustPressed(GLFW_KEY_P)) {
            std::cout << "Player position: ";
            std::cout << player.getCamera()->pos.x << " " 
                        << player.getCamera()->pos.y << " " 
                        << player.getCamera()->pos.z << std::endl;
            std::cout << "Player camera angle: ";
            std::cout << player.getCamera()->getYaw() << " "
                        << player.getCamera()->getPitch() << std::endl;
        }

        player.setPos(newPos);
        player.update(deltaTime);

        if (inputController.isKeyPressed(GLFW_KEY_F1)) 
            renderingSystem.setDrawMode(0);
        if (inputController.isKeyPressed(GLFW_KEY_F2)) 
            renderingSystem.setDrawMode(1);
        if (inputController.isKeyPressed(GLFW_KEY_F3)) 
            renderingSystem.setDrawMode(2);

        if (inputController.isKeyJustPressed(GLFW_KEY_ESCAPE)) {
            if (win.getCursorInputMode() == GLFW_CURSOR_NORMAL) {
                win.setCursorInputMode(GLFW_CURSOR_DISABLED);
            } else {
                win.setCursorInputMode(GLFW_CURSOR_NORMAL);
            }
            isInGame = !isInGame;
        }

        globalMaterialBuffer.sync();
        renderingSystem.render(deltaTime);

        win.swapBuffers();
    }

    return 0;
}
