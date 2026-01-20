#include "InputContoller.hpp"

InputController::InputController() {
    memset(pressed, false, KEY_BUFFER_SIZE * sizeof(bool));
    memset(frames, 0, KEY_BUFFER_SIZE * sizeof(uint32_t));
}