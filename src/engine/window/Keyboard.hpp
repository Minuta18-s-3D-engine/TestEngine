#ifndef ENGINE_WINDOW_KEYBOARD_H_
#define ENGINE_WINDOW_KEYBOARD_H_

#include <cstring>
#include <stdint.h>

const int KEYBOARD_BUFFER_SIZE = 1024;

class Keyboard {
    bool pressed[KEYBOARD_BUFFER_SIZE];
    uint32_t frames[KEYBOARD_BUFFER_SIZE];
public:
    Keyboard();
};

#endif // ENGINE_WINDOW_KEYBOARD_H_
