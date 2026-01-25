#ifndef ENGINE_WINDOW_INPUTCONTROLLER_H_
#define ENGINE_WINDOW_INPUTCONTROLLER_H_

#include <cstring>
#include <stdint.h>

#include "glm/glm.hpp"

class InputController {
    const static int KEYBOARD_BUTTONS = 1024;
    const static int MOUSE_BUTTONS = 32;
    const static int KEY_BUFFER_SIZE = KEYBOARD_BUTTONS + MOUSE_BUTTONS; 

    bool pressed[KEY_BUFFER_SIZE];
    uint32_t frames[KEY_BUFFER_SIZE];

    glm::vec2 mousePosition;
    glm::vec2 lastMousePosition;

    bool isCursorPinned;
public:
    InputController();

    glm::vec2 getMousePosition() const;
    glm::vec2 getMouseMovement() const;

    bool isKeyPressed(int keyCode) const;
    bool isKeyJustPressed(int keyCode) const;
    bool isMousePressed(int keyCode) const;
    bool isMouseJustPressed(int keyCode) const;

    bool isCursorPinned() const;
    void pinCursor(bool shouldPin) const;
};

#endif // ENGINE_WINDOW_INPUTCONTROLLER_H_
