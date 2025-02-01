#ifndef ENGINE_WINDOW_USERINPUT_H_
#define ENGINE_WINDOW_USERINPUT_H_

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <vector>
#include "../utils/EngineTypes.h"

const int KEYS_BUFFER_SIZE = 1024;
const int MOUSE_BUTTONS_BUFFER_SIZE = 12;
const int BUFFER_SIZE = KEYS_BUFFER_SIZE + MOUSE_BUTTONS_BUFFER_SIZE;

// Thanks to https://github.com/MihailRis/VoxelEngine-Cpp/src/window/Events.cpp
// for inspiration (Credits: MihailRis)

void key_callback(GLFWwindow* window, int button, int action, int mode);


class UserInput {
public:
    static uint current_frame;
    static std::vector<int> keys_pressed;

    static uint frames[BUFFER_SIZE];
    static bool pressed[BUFFER_SIZE];

    static void initialize();
    static void terminate();

    static void pollEvents();

    static bool isKeyPressed(int key_code);
    static bool isKeyJustPressed(int key_code);
    static bool isMousePressed(int key_code);
    static bool isMouseJustPressed(int key_code);
};

#endif // ENGINE_WINDOW_USERINPUT_H_
