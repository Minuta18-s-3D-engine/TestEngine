#ifndef ENGINE_GAMEOBJECT_COMPONENTS_TRANSFORM_H_
#define ENGINE_GAMEOBJECT_COMPONENTS_TRANSFORM_H_

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include <memory>

#include "../Component.hpp"

class Transform : public Component {
public:
    glm::vec3 position{ 0.0f };
    glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
    glm::vec3 scale{ 1.0f };

    std::unique_ptr<Component> clone() override {
        auto ptr = std::make_unique<Transform>();
        ptr->position = position;
        ptr->rotation = rotation;
        ptr->scale = scale;
        return ptr;
    }
};

#endif // ENGINE_GAMEOBJECT_COMPONENTS_TRANSFORM_H_
