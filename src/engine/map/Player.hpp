#ifndef ENGINE_GRAPHICS_PLAYER_H_
#define ENGINE_GRAPHICS_PLAYER_H_

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "../graphics/Camera.hpp"

#include <memory>
#include <iostream>

class Player {
    Camera camera;
    glm::vec3 pos;
    const float PLAYER_SPEED = 5.0f;
public:
    Player();
    ~Player();

    std::unique_ptr<Camera> getCamera();

    void addPos(const glm::vec3& acc);
    void setPos(const glm::vec3& acc);
    float getSpeed();

    void update(float deltaTime);
};

#endif // ENGINE_GRAPHICS_PLAYER_H_
