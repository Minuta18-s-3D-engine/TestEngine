#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#include "engine/graphics/Shader.hpp"

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

void framebufferSizeCallback(GLFWwindow* win, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* win) {
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(win, true);
    }
}

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
};

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
        WINDOW_WIDTH, WINDOW_HEIGHT, "TestEng", nullptr, nullptr
    );

    if (window == nullptr) {
        std::cerr << "Failed to create GLFW Window" << std::endl;
        return 0;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)  glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return 0;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // std::string vertex_shader_text = "";
    // std::string line;
    // std::ifstream vertex_shader("./assets/shaders/vertex.glsl");
    // while (getline(vertex_shader, line)) {
    //     vertex_shader_text += line;
    //     vertex_shader_text += "\n";
    // }
    // vertex_shader.close();

    // unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    // const char* vertex_shader_text_real = vertex_shader_text.c_str(); 
    // glShaderSource(vertex_shader_id, 1, &vertex_shader_text_real, nullptr);
    // glCompileShader(vertex_shader_id);

    // int success;
    // char infoLog[512];
    // glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);

    // if (!success) {
    //     glGetShaderInfoLog(vertex_shader_id, 512, nullptr, infoLog);
    //     std::cerr << "Vertex shader compilation failed:" << std::endl;
    //     std::cerr << infoLog << std::endl;
    //     glfwTerminate();
    //     return 0;
    // } 

    // std::string fragment_shader_text = "";
    // std::ifstream fragment_shader("./assets/shaders/fragment.glsl");
    // while (getline(fragment_shader, line)) {
    //     fragment_shader_text += line;
    //     fragment_shader_text += "\n";
    // }
    // fragment_shader.close();

    // unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    // const char* fragment_shader_text_real = fragment_shader_text.c_str(); 
    // glShaderSource(fragment_shader_id, 1, &fragment_shader_text_real, nullptr);
    // glCompileShader(fragment_shader_id);

    // glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);

    // if (!success) {
    //     glGetShaderInfoLog(fragment_shader_id, 512, nullptr, infoLog);
    //     std::cerr << "fragment shader compilation failed: " << std::endl;
    //     std::cerr << infoLog << std::endl;
    //     glfwTerminate();
    //     return 0;
    // } 

    // unsigned int shaderProgramId;
    // shaderProgramId = glCreateProgram();
    // glAttachShader(shaderProgramId, vertex_shader_id);
    // glAttachShader(shaderProgramId, fragment_shader_id);
    // glLinkProgram(shaderProgramId);

    // glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
    // if (!success) {
    //     glGetProgramInfoLog(fragment_shader_id, 512, nullptr, infoLog);
    //     std::cerr << "shader program linking failed" << std::endl;
    //     glfwTerminate();
    //     return 0;
    // }    

    // glDeleteProgram(vertex_shader_id);
    // glDeleteProgram(fragment_shader_id);

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

    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glUseProgram(shaderProgramId);
        main_shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // glDeleteProgram(shaderProgramId);

    glfwTerminate();

    return 0;
}
