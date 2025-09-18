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
#include "engine/assets/Files.hpp"
#include "engine/graphics/Texture.hpp"
#include "engine/player/Player.hpp"
#include "engine/materials/Material.hpp"
#include "engine/materials/Light.hpp"
#include "engine/materials/TextureMaterial.hpp"
#include "engine/models/Mesh.hpp"

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

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

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

const float MOUSE_SENSITIVITY = 0.1;

int main() {
    try {
        UserInput::initialize();
        Window::initialize(WINDOW_WIDTH, WINDOW_HEIGHT, "TestEng");

        size_t len = 0;
        auto texture_content = read_bytes("assets/textures/container.png", len);
        auto texture = PngCodec::load_texture(
            texture_content.get(), len, "container");

        size_t len2 = 0;
        auto texture_content2 = read_bytes("assets/textures/containerSpecular.png", len2);
        auto texture2 = PngCodec::load_texture(
            texture_content2.get(), len2, "containerSpec");

        TextureMaterial objTexture(texture, TextureType::DIFFUSE);
        TextureMaterial specTexture(texture2, TextureType::SPECULAR);

        Player player(glm::vec3(0.0f, 0.0f, -1.0f));
        Shader lightingShader("main");
        Shader lightSourceShader("lightSource");

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

        Mesh cubeMesh(cubeVertices, cubeIndices, cubeTextures);

        uint lightVAO, lightVBO;
        glGenVertexArrays(1, &lightVAO);
        glGenBuffers(1, &lightVBO);
        glBindVertexArray(lightVAO);
        glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

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

            // Отрисовка куба

            glm::mat4 view = player.getCamera()->getViewMat();
            glm::mat4 worldModel = glm::mat4(1.0f);

            lightingShader.use();
            lightingShader.setUniform4mat("projection", proj);
            lightingShader.setUniform4mat("view", view);
            lightingShader.setUniform3f("viewPos", player.getPos());
            
            l1.passToShader(lightingShader);
            mat1.passToShader(lightingShader);

            glm::mat4 model = glm::translate(worldModel, cubePositions[0]);
            lightingShader.setUniform4mat("model", model);

            cubeMesh.draw(lightingShader);

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
    } catch (std::runtime_error err) {
        // Just skipping to exit
        std::cerr << "Critical error: runtime_error:" << std::endl;
        std::cerr << "\t" << err.what() << std::endl;
    }

    UserInput::terminate();
    Window::terminate();

    return 0;
}
