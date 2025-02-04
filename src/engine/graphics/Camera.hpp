#ifndef ENGINE_GRAPHICS_CAMERA_H_
#define ENGINE_GRAPHICS_CAMERA_H_

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

class Camera {
    float zoom, fov;
    float yaw, pitch;

    void updateCameraVectors();
public:
    int test;
    glm::vec3 pos, front, up, right, worldUp;

    Camera(
        glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), 
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
        float yaw = 90.0f, 
        float pitch = 0.0f, float fov = 90.0f);
    ~Camera();

    float getFov();
    float getZoom();
    void processMouseMovement(float xOffset, float yOffset, float sensitivity);

    glm::mat4 getViewMat();
};

#endif // ENGINE_GRAPHICS_CAMERA_H_
