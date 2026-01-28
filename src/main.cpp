#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <memory>
#include <filesystem>
#include <sstream>
#include <chrono>

#include "engine/graphics/Shader.hpp"
#include "engine/graphics/ComputeShader.hpp"
#include "engine/window/Window.hpp"
#include "engine/window/InputController.hpp"
#include "engine/assets/coders/images/PngCoder.hpp"
#include "engine/assets/utils/Files.hpp"
#include "engine/assets/AssetManager.hpp"
#include "engine/graphics/Texture.hpp"
#include "engine/player/Player.hpp"
#include "engine/materials/Material.hpp"
#include "engine/materials/Light.hpp"
#include "engine/materials/TextureMaterial.hpp"
#include "engine/models/Mesh.hpp"
#include "engine/scene/Scene.hpp"
#include "engine/models/Model.hpp"
#include "engine/models/ModelManager.hpp"
#include "engine/models/ModelComponent.hpp"
#include "engine/gameobject/GameObjectManager.hpp"
#include "engine/gameobject/GameObject.hpp"
#include "engine/graphics/components/PointLight.hpp"
#include "engine/graphics/RenderingSystem.hpp"
#include "engine/models/ModelLoader.hpp"
#include "engine/assets/utils/MeshGen.hpp"

namespace fs = std::filesystem;

Light l1(glm::vec3(1.5f, 2.0f, 3.0f), glm::vec3(1.0f, 1.0f, 1.0f));

const float MOUSE_SENSITIVITY = 0.15;

// temporary solution
void createRect(
    glm::vec3 pos, glm::vec3 scale, glm::vec2 textureScale, Material mat, 
    std::string textureKey, AssetManager& manager,
    GameObjectManager& objectManager, ModelManager& modelManager
) {
    std::shared_ptr<Mesh> cubeMesh = generateCubeMesh(
        scale, textureScale, textureKey, manager, mat
    );

    std::vector<std::shared_ptr<Mesh>> cubeMeshArray;
    cubeMeshArray.push_back(cubeMesh);
    std::unique_ptr<Model> cubeModel = std::make_unique<Model>(cubeMeshArray);
    
    auto modelId = modelManager.add(cubeModel);

    std::unique_ptr<GameObject> cubeObject = GameObject::createGameObject();
    auto transformComponent = cubeObject->getComponent<Transform>();
    transformComponent->position = pos;
    transformComponent->scale = scale;
    auto behaviorComponent = cubeObject->getComponent<Behavior>();
    behaviorComponent->type = BehaviorType::STATIC;
    auto modelComponent = std::make_unique<ModelComponent>();
    modelComponent->managerId = modelId;
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

void loadTexture(
    std::string path, std::string key, std::string material_key, 
    const TextureType& type, AssetManager& manager
) {
    size_t len = 0;
    auto texture_content = read_bytes(path, len);
    auto texture = PngCoder::load_texture(
        texture_content.get(), len, key);
    manager.set<Texture>(texture, key);

    auto mat = std::make_shared<TextureMaterial>(texture, type);
    manager.set<TextureMaterial>(
        mat, material_key
    );
}

int main() {
    EventManager eventManager;
    Window win(eventManager);
    InputController& inputController = win.getInputController();

    AssetManager assetManager;

    std::string path = "./assets/textures";
    for (const auto & entry : fs::directory_iterator(path)) {
        fs::path p = entry.path();
        std::string stem = p.stem().string();
        std::string ending = "Specular";

        bool isSpecular = true;
        if (stem.size() < ending.size()) {
            isSpecular = false;
        } else {
            isSpecular = (0 == stem.compare(
                stem.size() - ending.size(), ending.size(), ending
            ));
        }

        std::cout << "textures/" + stem << std::endl;
        loadTexture(
            "assets/textures/" + p.filename().string(), 
            "textures/" + stem, 
            "materials/" + stem, 
            isSpecular ? TextureType::SPECULAR : TextureType::DIFFUSE, 
            assetManager
        );
    }

    Shader geomShader("geomShader");
    Shader lightingShader("lightingShader");

    assetManager.set<Shader>(std::make_shared<Shader>(geomShader), "shaders/geomShader");
    assetManager.set<Shader>(std::make_shared<Shader>(lightingShader), "shaders/lightingShader");

    ComputeShader buildClustersShader("buildClusters"), 
        lightCullingShader("lightCulling");

    assetManager.set<ComputeShader>(std::make_shared<ComputeShader>(buildClustersShader), "shaders/buildClusters");
    assetManager.set<ComputeShader>(std::make_shared<ComputeShader>(lightCullingShader), "shaders/lightCulling");

    GameObjectManager objectManager;
    ModelManager modelManager;
    RenderingSystem renderingSystem(
        assetManager,
        objectManager,
        modelManager,
        eventManager,
        win
    );

    Player player(glm::vec3(0.0f, 2.0f, -1.0f));
    player.setPos(glm::vec3(37.153, 4.26025, 3.34797));
    player.getCamera()->setRotation(198.795, 4.16543);
    renderingSystem.bindCamera(player.getCamera().get());

    createRect(
        glm::vec3(2.0, 2.0, 5.0), glm::vec3(1.0, 1.0, 1.0), 
        glm::vec2(1.0, 1.0), Material(), "materials/container",
        assetManager, objectManager, modelManager
    );

    ModelLoader modelLoader;
    
    {
        auto sponzaModel = modelLoader.loadModel("assets/models/sponza_low_res.glb");
        auto modelId = modelManager.add(sponzaModel);

        std::unique_ptr<GameObject> sponzaObject = GameObject::createGameObject();
        auto transformComponent = sponzaObject->getComponent<Transform>();
        transformComponent->position = glm::vec3(0.0f, 1.0f, 0.0f);
        transformComponent->scale = glm::vec3(1.0f, 1.0f, 1.0f);
        auto behaviorComponent = sponzaObject->getComponent<Behavior>();
        behaviorComponent->type = BehaviorType::STATIC;
        auto modelComponent = std::make_unique<ModelComponent>();
        modelComponent->managerId = modelId;
        sponzaObject->addComponent<ModelComponent>(modelComponent);

        objectManager.addObject(sponzaObject);
    }

    std::ifstream lightsFile("assets/lights.txt");
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
            win.setCaption("TestEng (fps = " + std::to_string(framesCount) + ")");
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

        renderingSystem.update();

        win.swapBuffers();
    }

    return 0;
}
