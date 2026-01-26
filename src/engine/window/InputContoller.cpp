#include "InputContoller.hpp"

InputController::InputController() {
    memset(pressed, false, KEY_BUFFER_SIZE * sizeof(bool));
    memset(frames, 0, KEY_BUFFER_SIZE * sizeof(uint32_t));
}

glm::vec2 InputController::getMousePosition() const {
    return mousePosition;
}

glm::vec2 InputController::getMouseMovement() const {
    return mousePosition - lastMousePosition;
}

bool InputController::isKeyCodeValid(int keyCode) const {
    if (keyCode < 0) return false;
    if (keyCode >= KEY_BUFFER_SIZE) return false;
    return true;
}

bool InputController::isKeyPressed(int keyCode) const {
    if (!isKeyCodeValid(keyCode)) return false;
    return pressed[keyCode];
}
