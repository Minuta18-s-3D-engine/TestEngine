#ifndef ENGINE_GRAPHICS_WINDOW_H_
#define ENGINE_GRAPHICS_WINDOW_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <iostream>

#include "../utils/EngineTypes.h"

class Window {
    static GLFWwindow* window;
    
    static void checkGlad();
public:
    static int width, height;
    static std::string caption;

    static void initialize(uint width, uint height, std::string caption);
    static void terminate();

    static bool isShouldClose();
    static void setShouldClose(bool should_close);

    static void clearColor(glm::vec3 color);
    static void clearColor(glm::vec4 color);
    static void setViewport(int x, int y, int width, int height);
};

#endif // ENGINE_GRAPHICS_WINDOW_H_
