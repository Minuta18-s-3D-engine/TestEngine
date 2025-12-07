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

#include "engine/graphics/Shader.hpp"
#include "engine/graphics/ComputeShader.hpp"
#include "engine/window/Window.hpp"
#include "engine/window/UserInput.hpp"
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

namespace fs = std::filesystem;

const int INITIAL_WINDOW_WIDTH = 1920;
const int INITIAL_WINDOW_HEIGHT = 1080;

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

Light l1(glm::vec3(1.5f, 2.0f, 3.0f), glm::vec3(1.0f, 1.0f, 1.0f));

const float MOUSE_SENSITIVITY = 0.1;

// temporary solution
void createRect(
    glm::vec3 pos, glm::vec3 scale, glm::vec2 textureScale, Material mat, 
    std::string textureKey, AssetManager& manager,
    GameObjectManager& objectManager, ModelManager& modelManager
) {
    std::vector<Vertex> cubeVertices;
    for (int i = 0; i < 36; ++i) {
        Vertex v;
        v.pos = glm::vec3(
            vertices[i * 8] * scale.x, 
            vertices[i * 8 + 1] * scale.y, 
            vertices[i * 8 + 2] * scale.z
        );
        v.normal = glm::vec3(
            vertices[i * 8 + 3], 
            vertices[i * 8 + 4], 
            vertices[i * 8 + 5]
        );
        v.texCords = glm::vec2(
            vertices[i * 8 + 6] * textureScale.x, 
            vertices[i * 8 + 7] * textureScale.y
        );
        cubeVertices.push_back(v);
    }

    std::vector<uint> cubeIndices(36);
    for (int i = 0; i < 36; ++i) cubeIndices[i] = i;
    std::vector<TextureMaterial> cubeTextures = { 
        manager.require<TextureMaterial>(textureKey), 
        manager.require<TextureMaterial>(textureKey + "Specular")
    };

    std::shared_ptr<Mesh> cubeMesh = std::make_shared<Mesh>(
        cubeVertices, cubeIndices, cubeTextures, mat);
    // manager.set(std::make_shared<Mesh>(cubeMesh), "meshes/cubeMesh");

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
    auto texture = PngCodec::load_texture(
        texture_content.get(), len, key);
    manager.set<Texture>(texture, key);

    auto mat = std::make_shared<TextureMaterial>(texture, type);
    manager.set<TextureMaterial>(
        mat, material_key
    );
}

int main() {
    try {
        UserInput::initialize();
        Window::initialize(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, "TestEng");

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
            modelManager
        );

        

        Player player(glm::vec3(0.0f, 2.0f, -1.0f));
        renderingSystem.bindCamera(player.getCamera().get());

        createPointLight(
            glm::vec3(1.5f, 2.0f, 3.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.01f, 
            0.04f, objectManager
        );

        createPointLight(
            glm::vec3(-2.0, 2.0, -4.0), glm::vec3(1.0, 0.0, 0.0), 
            glm::pow(10.0f, -2.0f), glm::pow(10.0f, -0.5f), objectManager
        );

        glm::vec3 pos(0, 0, 0), scale(1, 1, 1);
        glm::vec2 textureScale(1, 1);
        Material mat(
            glm::vec3(1.0f, 0.5f, 0.35f), glm::vec3(0.2f, 0.2f, 0.2f), 
            glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f)
        );
        std::string textureKey = "materials/container";

        createRect(
            glm::vec3(0.0f, -0.6f, 0.0f),
            glm::vec3(1000.0f, 0.2, 1000.0f),
            glm::vec2(250.0f, 250.0f),
            Material(
                glm::vec3(0.8, 0.8, 0.8), 
                glm::vec3(0.05, 0.05, 0.05),
                glm::vec3(0.4, 0.4, 0.4),
                glm::vec3(0.4, 0.4, 0.4),
                16.0f
            ),
            "materials/pavingStone",
            assetManager, objectManager, modelManager
        );

        srand(time(NULL));
        for (int i = 0; i < 1500; ++i) {
            int size = rand() % 2 + 1;
            int tex = rand() % 2;
            createRect(
                glm::vec3(rand() % 500 - 250 + (rand() % 5) * 0.13, (size - 1.0) * 0.5 - ((rand() % 2) * 0.1), rand() % 500 - 250 + (rand() % 5) * 0.13),
                glm::vec3(size, size, size),
                glm::vec2(1.0, 1.0),
                Material(),
                (tex == 0 ? "materials/container" : "materials/paintedPlaster"),
                assetManager, objectManager, modelManager
            );
        }

        for (int i = 0; i < 1000; ++i) {
            float lightX = rand() % 500 - 250 + (rand() % 5) * 0.13;
            float lightY = 0.1 + rand() % 5;
            float lightZ = rand() % 500 - 250 + (rand() % 5) * 0.13;

            float lightColotR = 0.5 + (rand() % 100) * 0.005;
            float lightColotG = 0.5 + (rand() % 100) * 0.005;
            float lightColotB = 0.5 + (rand() % 100) * 0.005;

            float lightLinear = glm::pow(10.0f, -(rand() % 12 + 1) * 0.1);
            float lightQuadratic = glm::pow(10.0f, -(rand() % 12 + 1) * 0.1);
        
            createPointLight(
                glm::vec3(lightX, lightY, lightZ),
                glm::vec3(lightColotR, lightColotG, lightColotB),
                lightLinear, lightQuadratic, objectManager
            );
        }

        renderingSystem.updateCache();

        float lastFrame = 0.0f, currentFrame = 0.0f, deltaTime;
        float lastFPSDisplay = -10.0f;
        int framesCount = 0;
        bool isInGame = true;
        while (!Window::isShouldClose()) {
            UserInput::pollEvents();

            if (isInGame) {
                player.getCamera()->processMouseMovement(
                    UserInput::getMouseXMov(), -UserInput::getMouseYMov(), 
                    MOUSE_SENSITIVITY
                );
            }

            currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            framesCount += 1;
            if (currentFrame - lastFPSDisplay >= 1.0) {
                Window::setTitle("TestEng (fps = " + std::to_string(framesCount) + ")");
                framesCount = 0;
                lastFPSDisplay = currentFrame;
            }

            glm::vec3 frontVec = glm::normalize(player.getCamera()->front); 
                // TODO: move this logic to player class
            glm::vec3 rightVec = glm::normalize(player.getCamera()->right);
            glm::vec3 newPos = player.getPos();
            if (UserInput::isKeyPressed(GLFW_KEY_W)) 
                newPos += (frontVec * deltaTime * player.getSpeed());
            if (UserInput::isKeyPressed(GLFW_KEY_S))
                newPos -= (frontVec * deltaTime * player.getSpeed());
            if (UserInput::isKeyPressed(GLFW_KEY_A))
                newPos -= (rightVec * deltaTime * player.getSpeed());
            if (UserInput::isKeyPressed(GLFW_KEY_D))
                newPos += (rightVec * deltaTime * player.getSpeed());
            if (UserInput::isKeyPressed(GLFW_KEY_SPACE))
                newPos.y += deltaTime * player.getSpeed();
            if (UserInput::isKeyPressed(GLFW_KEY_LEFT_SHIFT))
                newPos.y -= deltaTime * player.getSpeed();

            player.setPos(newPos);
            player.update(deltaTime);

            if (UserInput::isKeyPressed(GLFW_KEY_F1)) 
                renderingSystem.setDrawMode(0);
            if (UserInput::isKeyPressed(GLFW_KEY_F2)) 
                renderingSystem.setDrawMode(1);
            if (UserInput::isKeyPressed(GLFW_KEY_F3)) 
                renderingSystem.setDrawMode(2);

            if (UserInput::isKeyJustPressed(GLFW_KEY_ESCAPE)) {
                if (Window::getCursorInputMode() == GLFW_CURSOR_NORMAL) {
                    Window::setCursorInputMode(GLFW_CURSOR_DISABLED);
                } else {
                    Window::setCursorInputMode(GLFW_CURSOR_NORMAL);
                }
                isInGame = !isInGame;
            }

            Window::clearColor(glm::vec3(0.0f, 0.0f, 0.0f));
            Window::clear();

            renderingSystem.update();

            Window::swapBuffers();
        }
    } catch (const std::runtime_error& err) {
        // Just skipping to exit
        std::cerr << "Critical error: runtime_error:" << std::endl;
        std::cerr << "\t" << err.what() << std::endl;
    }

    UserInput::terminate();
    Window::terminate();

    return 0;
}
