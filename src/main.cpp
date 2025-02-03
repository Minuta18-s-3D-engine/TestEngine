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
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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

int main() {
    try {
        UserInput::initialize();
        Window::initialize(WINDOW_WIDTH, WINDOW_HEIGHT, "TestEng");
        
        size_t len = 0;
        auto texture_content = read_bytes("assets/textures/container.png", len);
        auto texture = PngCodec::load_texture(
            texture_content.get(), len, "floor");

        Player player;
        Shader mainShader("main");

        unsigned int VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // glGenBuffers(1, &EBO);
        
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);

        mainShader.use();
        mainShader.setUniform1i("texture1", 0);

        float lastFrame = 0.0f, currentFrame = 0.0f, deltaTime;
        glm::vec3 keys(0.0f, 0.0f, 0.0f);

        while (!Window::isShouldClose()) {
            UserInput::pollEvents();

            currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            keys.x = 0.0f; keys.y = 0.0f; keys.z = 0.0f;
            if (UserInput::isKeyPressed(GLFW_KEY_W)) keys.z -= 1.0f;
            if (UserInput::isKeyPressed(GLFW_KEY_S)) keys.z += 1.0f;
            if (UserInput::isKeyPressed(GLFW_KEY_A)) keys.x -= 1.0f;
            if (UserInput::isKeyPressed(GLFW_KEY_D)) keys.x += 1.0f;
            if (UserInput::isKeyPressed(GLFW_KEY_SPACE)) keys.y += 1.0f;
            if (UserInput::isKeyPressed(GLFW_KEY_LEFT_SHIFT)) keys.y -= 1.0f;

            player.addPos(keys * deltaTime * player.getSpeed());
            player.update(deltaTime);

            if (UserInput::isKeyJustPressed(GLFW_KEY_ESCAPE)) 
                Window::setShouldClose(true);

            Window::clearColor(glm::vec3(0.2f, 0.3f, 0.3f));
            Window::clear();

            glActiveTexture(GL_TEXTURE0);
            texture->bind();

            mainShader.use();

            glm::mat4 view = player.getCamera()->getViewMat();
            glm::mat4 proj = glm::mat4(1.0f);

            proj = glm::perspective(
                glm::radians(45.0f), 
                (float) Window::width / (float) Window::height, 
                0.1f, 100.0f
            );

            mainShader.setUniform4mat("projection", proj); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
            mainShader.setUniform4mat("view", view);

            glBindVertexArray(VAO);

            for (uint i = 0; i < 10; i++) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                mainShader.setUniform4mat("model", model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

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
