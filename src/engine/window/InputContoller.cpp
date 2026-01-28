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

