#ifndef ENGINE_WINDOW_INPUTCONTROLLER_H_
#define ENGINE_WINDOW_INPUTCONTROLLER_H_

#include <cstring>
#include <stdint.h>

class InputController {
    const static int KEYBOARD_BUTTONS = 1024;
    const static int MOUSE_BUTTONS = 32;
    const static int KEY_BUFFER_SIZE = KEYBOARD_BUTTONS + MOUSE_BUTTONS; 

    bool pressed[KEY_BUFFER_SIZE];
    uint32_t frames[KEY_BUFFER_SIZE];
public:
    InputController();
};

#endif // ENGINE_WINDOW_INPUTCONTROLLER_H_
