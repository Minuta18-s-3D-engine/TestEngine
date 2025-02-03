#include "Camera.hpp"

Camera::Camera() {
    pos = glm::vec3(0.0f, 0.0f, 3.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f,  0.0f);
}

Camera::~Camera() = default;

glm::mat4 Camera::getViewMat() {
    return glm::lookAt(pos, pos + front, up);
}

