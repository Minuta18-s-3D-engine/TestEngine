#include "Keyboard.hpp"

Keyboard::Keyboard() {
    memset(pressed, false, KEYBOARD_BUFFER_SIZE * sizeof(bool));
    memset(frames, 0, KEYBOARD_BUFFER_SIZE * sizeof(uint32_t));
}