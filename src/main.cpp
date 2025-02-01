#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#include "engine/graphics/Shader.hpp"
#include "engine/window/Window.hpp"
#include "engine/window/UserInput.hpp"

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
};

int main() {
    try {
        UserInput::initialize();
        Window::initialize(WINDOW_WIDTH, WINDOW_HEIGHT, "TestEng");
        
        
        Shader main_shader("main");

        unsigned int VAO, VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        while (!Window::isShouldClose()) {
            UserInput::pollEvents();
            // processInput(window);

            if (UserInput::isKeyJustPressed(GLFW_KEY_ESCAPE)) 
                Window::setShouldClose(true);

            // glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
            // glClear(GL_COLOR_BUFFER_BIT);

            Window::clearColor(glm::vec3(0.2f, 0.2f, 0.3f));
            Window::clear();

            main_shader.use();
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            Window::swapBuffers();
        }

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    } catch (std::runtime_error) {
        // Just skipping to exit
    }

    UserInput::terminate();
    Window::terminate();

    return 0;
}
