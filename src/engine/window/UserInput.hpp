#ifndef ENGINE_WINDOW_USERINPUT_H_
#define ENGINE_WINDOW_USERINPUT_H_

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <vector>
#include "../utils/EngineTypes.h"
#include "Window.hpp"

const int KEYS_BUFFER_SIZE = 1024;
const int MOUSE_BUTTONS_BUFFER_SIZE = 12;
const int BUFFER_SIZE = KEYS_BUFFER_SIZE + MOUSE_BUTTONS_BUFFER_SIZE;

// Thanks to https://github.com/MihailRis/VoxelEngine-Cpp/src/window/Events.cpp
// for inspiration (Credits: MihailRis)

void key_callback(GLFWwindow* window, int button, 
    int scancode, int action, int mode);

void mouse_button_callback(GLFWwindow*, int button, int action, int);

void cursor_position_callback(GLFWwindow*, double x, double y);

class UserInput {
public:
    static bool isCursorPinned;

    static uint current_frame;
    static std::vector<int> keys_pressed;

    static uint frames[BUFFER_SIZE];
    static bool pressed[BUFFER_SIZE];

    static double last_x_mouse_pos, last_y_mouse_pos;
    static double x_mouse_pos, y_mouse_pos;

    static void initialize();
    static void terminate();

    static void pollEvents();

    static double getMouseXMov();
    static double getMouseYMov();

    static bool isKeyPressed(int key_code);
    static bool isKeyJustPressed(int key_code);
    static bool isMousePressed(int key_code);
    static bool isMouseJustPressed(int key_code);

    static bool getIsCursorPinned();
    static void setIsCursorPinned(bool value);
};

#endif // ENGINE_WINDOW_USERINPUT_H_
