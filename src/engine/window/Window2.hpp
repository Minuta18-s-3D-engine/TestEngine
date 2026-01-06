#ifndef ENGINE_GRAPHICS_WINDOW2_H_
#define ENGINE_GRAPHICS_WINDOW2_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <functional>

#include "../utils/EngineTypes.h"

class Window {
    typedef std::function<void(GLFWwindow*, int, int)> FramebufferCallback;

    static GLFWwindow* window;

    static void checkGlad();
public:
    static std::vector<FramebufferCallback> framebufferSizeCallbacks;
    static int width, height;
    static std::string caption;
    static int cursorInputMode;

    static void initialize(uint width, uint height, std::string caption);
    static void terminate();
    
    static void setCursorInputMode(int mode);
    static int getCursorInputMode();
    static bool isShouldClose();
    static void setShouldClose(bool should_close);

    static void clearColor(glm::vec3 color);
    static void clearColor(glm::vec4 color);
    static void clear();
    static void setTitle(std::string newTitle);
    static void swapBuffers();
    static void setViewport(int x, int y, int width, int height);

    static void addframebufferCallback(FramebufferCallback callback);
};

#endif // ENGINE_GRAPHICS_WINDOW2_H_
