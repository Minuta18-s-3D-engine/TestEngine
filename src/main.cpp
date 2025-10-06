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
#include "engine/scene/StaticObject.hpp"
#include "engine/models/Model.hpp"

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
    std::string textureKey, AssetManager& manager, Scene& scene 
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
    std::shared_ptr<Model> cubeModel = std::make_shared<Model>(cubeMeshArray);
    std::shared_ptr<StaticObject> cubeObject = std::make_shared<StaticObject>(
        cubeModel, pos);

    scene.addObject(cubeObject);
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

        Player player(glm::vec3(0.0f, 0.0f, -1.0f));
        Shader lightingShader("forward/main");
        Shader lightSourceShader("forward/lightSource");

        assetManager.set<Shader>(std::make_shared<Shader>(lightingShader), "shaders/forward/main");
        assetManager.set<Shader>(std::make_shared<Shader>(lightSourceShader), "shaders/forward/lightSource");

        Shader geomShader("gbuffer/geomShader");
        Shader lightingShader("gbuffer/lightingShader");

        assetManager.set<Shader>(std::make_shared<Shader>(geomShader), "shaders/gbuffer/geomShader");
        assetManager.set<Shader>(std::make_shared<Shader>(lightingShader), "shaders/gbuffer/lightingShader");

        Scene mainScene(assetManager);
        std::shared_ptr<Light> l1Ptr = 
            std::make_shared<Light>(l1);
        mainScene.addLight(l1Ptr);
        mainScene.addLight(Light::calcLight(
            glm::vec3(-2.0, 1.0, -4.0),
            glm::vec3(1.0, 0.0, 0.0),
            2.0, 0.5
        ));

        glm::vec3 pos(0, 0, 0), scale(1, 1, 1);
        glm::vec2 textureScale(1, 1);
        Material mat(
            glm::vec3(1.0f, 0.5f, 0.35f), glm::vec3(0.2f, 0.2f, 0.2f), 
            glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f)
        );
        std::string textureKey = "materials/container";
        AssetManager& manager = assetManager;

        // createRect(pos, scale, textureScale, mat, textureKey, 
        //     assetManager, mainScene);
        createRect(
            glm::vec3(0.0f, -0.6f, 0.0f),
            glm::vec3(200.0f, 0.2, 200.0f),
            glm::vec2(50.0f, 50.0f),
            Material(
                glm::vec3(0.8, 0.8, 0.8), 
                glm::vec3(0.05, 0.05, 0.05),
                glm::vec3(0.4, 0.4, 0.4),
                glm::vec3(0.4, 0.4, 0.4),
                16.0f
            ),
            "materials/pavingStone",
            assetManager, mainScene
        );

        for (int i = 0; i < 500; ++i) {
            int size = rand() % 2 + 1;
            int tex = rand() % 2;
            createRect(
                glm::vec3(rand() % 100 - 50 + (rand() % 5) * 0.13, (size - 1.0) * 0.5 - ((rand() % 2) * 0.1), rand() % 100 - 50 + (rand() % 5) * 0.13),
                glm::vec3(size, size, size),
                glm::vec2(1.0, 1.0),
                Material(),
                (tex == 0 ? "materials/container" : "materials/paintedPlaster"),
                assetManager, mainScene
            );
        }

        // for (int i = 0; i < 100; ++i) {
        //     mainScene.addLight(Light::calcLight(
        //         glm::vec3(rand() % 100 - 50 + (rand() % 5) * 0.13, 0.1 + rand() % 5, rand() % 100 - 50 + (rand() % 5) * 0.13),
        //         glm::vec3(0.5 + (rand() % 100) * 0.005, 0.5 + (rand() % 100) * 0.005, 0.5 + (rand() % 100) * 0.005),
        //         (rand() % 6 + 1) * 0.1, (rand() % 6 + 1) * 0.1
        //     ));
        // }

        // createRect(
        //     glm::vec3(4.0f, 1.0f, 2.0f),
        //     glm::vec3(1.0f, 3.0f, 6.0f),
        //     glm::vec2(3.0f, 3.0f),
        //     Material(
        //         glm::vec3(1.0f, 0.5f, 0.35f), glm::vec3(0.1f, 0.1f, 0.1f), 
        //         glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.8f, 0.8f, 0.8f)
        //     ),
        //     "materials/bricksRed",
        //     assetManager, mainScene
        // );

        float lastFrame = 0.0f, currentFrame = 0.0f, deltaTime;
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

            if (UserInput::isKeyJustPressed(GLFW_KEY_ESCAPE)) {
                if (Window::getCursorInputMode() == GLFW_CURSOR_NORMAL) {
                    Window::setCursorInputMode(GLFW_CURSOR_DISABLED);
                } else {
                    Window::setCursorInputMode(GLFW_CURSOR_NORMAL);
                }
                isInGame = !isInGame;
            }

            Window::clearColor(glm::vec3(0.1f, 0.1f, 0.1f));
            Window::clear();

            mainScene.drawAll(player.getCamera().get());

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
