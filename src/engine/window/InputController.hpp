#ifndef ENGINE_WINDOW_INPUTCONTROLLER_H_
#define ENGINE_WINDOW_INPUTCONTROLLER_H_

#include "glm/glm.hpp"
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <cstring>
#include <stdint.h>

class Window;

class InputController {
    friend class Window;
    
    GLFWwindow* window;

    const static int KEYBOARD_BUTTONS = 1024;
    const static int MOUSE_BUTTONS = 32;
    const static int KEY_BUFFER_SIZE = KEYBOARD_BUTTONS + MOUSE_BUTTONS; 

    bool pressed[KEY_BUFFER_SIZE];
    uint32_t frames[KEY_BUFFER_SIZE];
    uint32_t currentFrame;

    glm::vec2 mousePosition = {0.0, 0.0};
    glm::vec2 lastMousePosition = {0.0, 0.0};

    bool cursorPositionSet = false;
    bool cursorPinned = false;

    bool isKeyCodeValid(int keyCode) const;
    bool isMouseCodeValid(int keyCode) const;

    void keyPressCallback(int _button, int _scancode, int _action, int _mode);
    void mouseButtonCallback(int _button, int _action, int _mode);
    void cursorPositionCallback(double _x, double _y);
public:
    InputController(GLFWwindow* _window);

    glm::vec2 getMousePosition() const;
    glm::vec2 getMouseMovement() const;

    bool isKeyPressed(int keyCode) const;
    bool isKeyJustPressed(int keyCode) const;
    bool isMousePressed(int keyCode) const;
    bool isMouseJustPressed(int keyCode) const;

    bool isCursorPinned() const;
    void pinCursor(bool shouldPin);

    void updateEvents();
};

#endif // ENGINE_WINDOW_INPUTCONTROLLER_H_
