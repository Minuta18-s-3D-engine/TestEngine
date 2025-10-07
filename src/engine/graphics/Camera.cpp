#include "Camera.hpp"

Camera::Camera(
    glm::vec3 pos, glm::vec3 up, float yaw, float pitch, 
    float fov, float zNear, float zFar 
) : front(glm::vec3(0.0f, 0.0f, -1.0f)), fov(fov), zoom(45.0f) {
    this->pos = pos;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    this->zNear = zNear;
    this->zFar = zFar;
    updateCameraVectors();
}

Camera::~Camera() = default;

glm::mat4 Camera::getViewMat() const {
    return glm::lookAt(pos, pos + front, up);
}

void Camera::updateCameraVectors() {
    glm::vec3 newFront = front;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

float Camera::getFov() const {
    return fov;
}

float Camera::getZoom() const {
    return zoom;
}

void Camera::processMouseMovement(
    float xOffset, float yOffset, float sensitivity
) {
    // std::cout << xOffset << " " << yOffset << std::endl;

    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw += xOffset;
    pitch += yOffset;

    pitch = std::min(pitch, 89.9f);
    pitch = std::max(pitch, -89.9f);

    updateCameraVectors();

    // std::cout << front.x << " " << front.y << " " << front.z << std::endl;
}

