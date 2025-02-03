#ifndef ENGINE_GRAPHICS_CAMERA_H_
#define ENGINE_GRAPHICS_CAMERA_H_

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Camera {
    float fov;
public:
    glm::vec3 pos, dir, up, right, front;

    Camera();
    ~Camera();

    glm::mat4 getViewMat();
};

#endif // ENGINE_GRAPHICS_CAMERA_H_
