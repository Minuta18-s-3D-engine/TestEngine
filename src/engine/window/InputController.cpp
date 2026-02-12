#include "InputController.hpp"

InputController::InputController(GLFWwindow* _window) : window(_window) {
    memset(pressed, false, KEY_BUFFER_SIZE * sizeof(bool));
    memset(frames, 0, KEY_BUFFER_SIZE * sizeof(uint32_t));
}

void InputController::keyPressCallback(
    int _button, int _scancode, int _action, int _mode
) {
    if (_action == GLFW_PRESS) {
        pressed[_button] = true;
        frames[_button] = currentFrame;
    } else if (_action == GLFW_RELEASE) {
        pressed[_button] = false;
        frames[_button] = currentFrame;
    }
}

void InputController::mouseButtonCallback(int _button, int _action, int _mode) {
    int bufferIndex = _button + KEYBOARD_BUTTONS;
    if (_action == GLFW_PRESS) {
        pressed[bufferIndex] = true;
        frames[bufferIndex] = currentFrame;
    } else if (_action == GLFW_RELEASE) {
        pressed[bufferIndex] = false;
        frames[bufferIndex] = currentFrame;
    }
}

void InputController::cursorPositionCallback(double _x, double _y) {
    mousePosition.x = _x;
    mousePosition.y = _y;
    cursorPositionSet = true;

    if (isCursorPinned()) {
        glfwSetCursorPos(window, lastMousePosition.x, lastMousePosition.y);
    }
}

glm::vec2 InputController::getMousePosition() const {
    if (!cursorPositionSet) return {0.0, 0.0};
    return mousePosition;
}

glm::vec2 InputController::getMouseMovement() const {
    if (!cursorPositionSet) return {0.0, 0.0};
    return mousePosition - lastMousePosition;
}

bool InputController::isKeyCodeValid(int keyCode) const {
    if (keyCode < 0) return false;
    if (keyCode >= KEYBOARD_BUTTONS) return false;
    return true;
}

bool InputController::isMouseCodeValid(int keyCode) const {
    if (keyCode < 0) return false;
    if (keyCode >= MOUSE_BUTTONS) return false;
    return true;
}

bool InputController::isKeyPressed(int keyCode) const {
    if (!isKeyCodeValid(keyCode)) return false;
    return pressed[keyCode];
}

bool InputController::isKeyJustPressed(int keyCode) const {
    if (!isKeyCodeValid(keyCode)) return false;
    return pressed[keyCode] && (frames[keyCode] == currentFrame);
}

bool InputController::isMousePressed(int mouseCode) const {
    if (!isMouseCodeValid(mouseCode)) return false;
    return pressed[mouseCode + KEYBOARD_BUTTONS];
}

bool InputController::isMouseJustPressed(int mouseCode) const {
    if (!isMouseCodeValid(mouseCode)) return false;
    return pressed[mouseCode + KEYBOARD_BUTTONS] && 
        (frames[mouseCode + KEYBOARD_BUTTONS] == currentFrame);
}

bool InputController::isCursorPinned() const {
    return cursorPinned;
}

void InputController::pinCursor(bool shouldPin) {
    cursorPinned = shouldPin;
}

void InputController::updateEvents() {
    lastMousePosition = mousePosition;
    currentFrame++;
    glfwPollEvents();
}

