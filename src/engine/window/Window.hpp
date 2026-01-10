#ifndef ENGINE_WINDOW_WINDOW_H_
#define ENGINE_WINDOW_WINDOW_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <cstdint>
#include <iostream>

#include "../events/EventManager.hpp"
#include "WindowEvents.hpp"

class Window {
    std::string caption;
    uint32_t width, height;
    int cursorInputMode;

    GLFWwindow* window;

    EventManager& eventManager;

    void setupWindow();
    static void framebufferSizeCallback(GLFWwindow* _window, int _width, int _height);
public:
    Window(
        uint32_t _width, 
        uint32_t _height, 
        const std::string& _caption, 
        EventManager& _eventManager
    );
    Window(EventManager& _eventManager);
    ~Window();

    bool isShouldClose() const;
    void setShouldClose(bool should);

    std::string getCaption() const;
    void setCaption(const std::string& _caption);
    uint32_t getHeight() const;
    uint32_t getWidth() const;
    void setViewport(uint32_t _width, uint32_t _height);
    int getCursorInputMode() const;
    void setCursorInputMode(int mode);

    void swapBuffers();
};

#endif // ENGINE_WINDOW_WINDOW_H_