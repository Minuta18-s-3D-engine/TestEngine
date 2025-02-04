#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#include "engine/graphics/Shader.hpp"
#include "engine/window/Window.hpp"
#include "engine/window/UserInput.hpp"
#include "engine/assets/coders/images/PngCoder.hpp"
#include "engine/assets/Files.hpp"
#include "engine/graphics/Texture.hpp"
#include "engine/map/Player.hpp"

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

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
            texture_content.get(), len, "floor");

        Player player(glm::vec3(0.0f, 0.0f, 0.0f));
        Shader lightingShader("main");
        Shader lightSourceShader("lightSource");

        unsigned int VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(VAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        uint lightVAO;
        glGenVertexArrays(1, &lightVAO);
        glBindVertexArray(lightVAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
        glEnableVertexAttribArray(0);


        float lastFrame = 0.0f, currentFrame = 0.0f, deltaTime;
        glm::vec3 keys(0.0f, 0.0f, 0.0f);

        while (!Window::isShouldClose()) {
            UserInput::pollEvents();

            player.getCamera()->processMouseMovement(
                UserInput::getMouseXMov(), -UserInput::getMouseYMov(), 
                MOUSE_SENSITIVITY
            );

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

            if (UserInput::isKeyJustPressed(GLFW_KEY_ESCAPE)) 
                Window::setShouldClose(true);

            Window::clearColor(glm::vec3(0.1f, 0.1f, 0.1f));
            Window::clear();

            lightingShader.use();
            
            lightingShader.setUniform3f("lightColor",  1.0f, 1.0f, 1.0f);

            glm::mat4 proj = glm::mat4(1.0f);
            proj = glm::perspective(
                player.getCamera()->getZoom(), 
                (float) Window::width / (float) Window::height, 
                0.1f, 100.0f
            );
            glm::mat4 view = player.getCamera()->getViewMat();

            lightingShader.setUniform4mat("projection", proj);
            lightingShader.setUniform4mat("view", view);

            glm::mat4 model = glm::translate(model, cubePositions[0]);
            lightingShader.setUniform4mat("model", model);
            lightingShader.setUniform3f("lightPos", 3.0f, 0.0f, 3.0f);

            // glActiveTexture(GL_TEXTURE0);
            // texture->bind();

            // glBindVertexArray(VAO);
            // glDrawArrays(GL_TRIANGLES, 0, 36);

            for (uint i = 0; i < 10; i++) {
                if (i % 3 == 0) {
                    lightingShader.setUniform3f("objectColor", 1.0f, 0.5f, 0.31f);
                } else if (i % 3 == 1) {
                    lightingShader.setUniform3f("objectColor", 0.5f, 0.31f, 1.0f);
                } else {
                    lightingShader.setUniform3f("objectColor", 0.31f, 1.0f, 0.5f);     
                }

                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                lightingShader.setUniform4mat("model", model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            lightSourceShader.use();
            lightSourceShader.setUniform4mat("projection", proj);
            lightSourceShader.setUniform4mat("view", view);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(3.0f, 0.0f, 3.0f));
            model = glm::scale(model, glm::vec3(0.2f));
            lightSourceShader.setUniform4mat("model", model);

            glBindVertexArray(lightVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            Window::swapBuffers();
        }

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    } catch (std::runtime_error err) {
        // Just skipping to exit
        std::cerr << "Critical error: runtime_error:" << std::endl;
        std::cerr << "\t" << err.what() << std::endl;
    }

    UserInput::terminate();
    Window::terminate();

    return 0;
}
