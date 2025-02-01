#include "UserInput.hpp"
#include <cstring>

uint UserInput::frames[BUFFER_SIZE];
bool UserInput::pressed[BUFFER_SIZE];
uint UserInput::current_frame = 0;
double UserInput::x_mouse_pos = 0;
double UserInput::y_mouse_pos = 0;

void key_callback(GLFWwindow* window, int button, int, int action, int) {
    if (action == GLFW_PRESS) {
        UserInput::pressed[button] = true;
        UserInput::frames[button] = UserInput::current_frame;
    } else if (action == GLFW_RELEASE) {
        UserInput::pressed[button] = false;
        UserInput::frames[button] = UserInput::current_frame;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int) {
    key_callback(window, button + KEYS_BUFFER_SIZE, 0, action, 0);
}

void cursor_position_callback(GLFWwindow* window, double x, double y) {
    UserInput::x_mouse_pos = x;
    UserInput::y_mouse_pos = y;
}

void UserInput::initialize() {
    memset(UserInput::pressed, false, KEYS_BUFFER_SIZE * sizeof(bool));
    memset(UserInput::frames, 0, KEYS_BUFFER_SIZE * sizeof(int));
}

void UserInput::terminate() {

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

