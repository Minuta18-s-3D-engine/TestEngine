#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <memory>

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

Material mat1(
    glm::vec3(1.0f, 0.5f, 0.35f), glm::vec3(0.1f, 0.1f, 0.1f), 
    glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f));
Material mat2(glm::vec3(0.35f, 1.0f, 0.5f));
Material mat3(glm::vec3(0.5f, 0.35f, 1.0f));
Light l1(glm::vec3(1.5f, 2.0f, 3.0f), glm::vec3(1.0f, 1.0f, 1.0f));

const float MOUSE_SENSITIVITY = 0.1;

// just for debug
void createRect(
    glm::vec3 pos, glm::vec3 scale, Material mat, std::string textureKey, 
    AssetManager& manager, Scene& scene 
) {
    
}

int main() {
    try {
        UserInput::initialize();
        Window::initialize(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, "TestEng");

        AssetManager assetManager;

        size_t len = 0;
        auto texture_content = read_bytes("assets/textures/container.png", len);
        auto texture = PngCodec::load_texture(
            texture_content.get(), len, "container");

        size_t len2 = 0;
        auto texture_content2 = read_bytes("assets/textures/containerSpecular.png", len2);
        auto texture2 = PngCodec::load_texture(
            texture_content2.get(), len2, "containerSpec");

        assetManager.set<Texture>(texture, "textures/container");
        assetManager.set<Texture>(texture, "textures/containerSpecular");

        TextureMaterial objTexture(texture, TextureType::DIFFUSE);
        TextureMaterial specTexture(texture2, TextureType::SPECULAR);

        assetManager.set<TextureMaterial>(std::make_shared<TextureMaterial>(objTexture), "materials/container");
        assetManager.set<TextureMaterial>(std::make_shared<TextureMaterial>(specTexture), "materials/containerSpecular");

        Player player(glm::vec3(0.0f, 0.0f, -1.0f));
        Shader lightingShader("main");
        Shader lightSourceShader("lightSource");

        assetManager.set<Shader>(std::make_shared<Shader>(lightingShader), "shaders/main");
        assetManager.set<Shader>(std::make_shared<Shader>(lightSourceShader), "shaders/lightSource");

        std::vector<Vertex> cubeVertices;
        for (int i = 0; i < 36; ++i) {
            Vertex v;
            v.pos = glm::vec3(
                vertices[i * 8], 
                vertices[i * 8 + 1], 
                vertices[i * 8 + 2]
            );
            v.normal = glm::vec3(
                vertices[i * 8 + 3], 
                vertices[i * 8 + 4], 
                vertices[i * 8 + 5]
            );
            v.texCords = glm::vec2(
                vertices[i * 8 + 6], 
                vertices[i * 8 + 7] 
            );
            cubeVertices.push_back(v);
        }

        std::vector<uint> cubeIndices(36);
        for (int i = 0; i < 36; ++i) cubeIndices[i] = i;
        std::vector<TextureMaterial> cubeTextures = 
            { objTexture, specTexture };

        Mesh cubeMesh(cubeVertices, cubeIndices, cubeTextures, mat1);
        assetManager.set(std::make_shared<Mesh>(cubeMesh), "meshes/cubeMesh");
        
        std::vector<std::shared_ptr<Mesh>> cubeMeshArray;
        cubeMeshArray.push_back(std::make_shared<Mesh>(cubeMesh));
        Model cubeModel(cubeMeshArray);
        StaticObject cubeObject(std::make_shared<Model>(cubeModel), {0.0f, 0.0f, 0.0f});

        //
        uint lightVAO, lightVBO;
        glGenVertexArrays(1, &lightVAO);
        glGenBuffers(1, &lightVBO);
        glBindVertexArray(lightVAO);
        glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        Scene mainScene(assetManager);
        std::shared_ptr<StaticObject> cubeObjectPtr = 
            std::make_shared<StaticObject>(cubeObject);
        mainScene.addObject(cubeObjectPtr);
        std::shared_ptr<Light> l1Ptr = 
            std::make_shared<Light>(l1);
        mainScene.addLight(l1Ptr);

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

            glm::mat4 proj = glm::mat4(1.0f);
            proj = glm::perspective(
                player.getCamera()->getZoom(), 
                (float) Window::width / (float) Window::height, 
                0.1f, 100.0f
            );
            glm::mat4 view = player.getCamera()->getViewMat();
            glm::mat4 worldModel = glm::mat4(1.0f);

            // Отрисовка куба

            mainScene.drawAll(player.getCamera().get());

            // Отрисовка источника освещения            

            glm::mat4 model2 = glm::translate(worldModel, l1.pos);
            model2 = glm::scale(model2, glm::vec3(0.2f));

            lightSourceShader.use();
            lightSourceShader.setUniform4mat("projection", proj);
            lightSourceShader.setUniform4mat("view", view);
            lightSourceShader.setUniform4mat("model", model2);
            lightSourceShader.setUniform3f("lightColor", l1.diffuse);
            lightSourceShader.setUniform3f("objectColor", 1.0f, 1.0f, 1.0f);

            glBindVertexArray(lightVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

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
