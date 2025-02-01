#include "UserInput.hpp"
#include <cstring>

uint UserInput::frames[BUFFER_SIZE];
bool UserInput::pressed[BUFFER_SIZE];

void key_callback(GLFWwindow* window, int button, int action, int mode) {
    if (action == GLFW_PRESS) {
        UserInput::pressed[button] = true;
        UserInput::frames[button] = UserInput::current_frame;
    } else if (action == GLFW_RELEASE) {
        UserInput::pressed[button] = false;
        UserInput::frames[button] = UserInput::current_frame;
    }
}

void UserInput::initialize() {
    UserInput::current_frame = 0;
    memset(UserInput::pressed, false, KEYS_BUFFER_SIZE * sizeof(bool));
    memset(UserInput::frames, 0, KEYS_BUFFER_SIZE * sizeof(int));
}

void UserInput::pollEvents() {
    UserInput::current_frame++;
    glfwPollEvents();
}

bool UserInput::isKeyPressed(int key_code) {
    return UserInput::pressed[key_code];
}

bool UserInput::isKeyJustPressed(int key_code) {
    return UserInput::pressed[key_code] && 
        (UserInput::frames[key_code] == UserInput::current_frame);
}

bool UserInput::isMousePressed(int key_code) {
    return UserInput::pressed[key_code + KEYS_BUFFER_SIZE];
}

bool UserInput::isMouseJustPressed(int key_code) {
    return UserInput::pressed[key_code + KEYS_BUFFER_SIZE] && 
        (UserInput::frames[key_code + KEYS_BUFFER_SIZE] == 
            UserInput::current_frame);
}

