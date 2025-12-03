#ifndef ENGINE_GRAPHICS_COMPONENTS_POINTLIGHT_H_
#define ENGINE_GRAPHICS_COMPONENTS_POINTLIGHT_H_

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include <memory>

#include "../../gameobject/Component.hpp"

class PointLight : public Component {
public:
    glm::vec3 color{ 1.0f };
    float linear;
    float quadratic;

    PointLight();
    PointLight(
        glm::vec3 _color,
        float _linear,
        float _quadratic
    );
    PointLight(
        glm::vec3 _color 
    );

    std::unique_ptr<Component> clone() override {
        auto ptr = std::make_unique<PointLight>();
        ptr->color = color;
        ptr->linear = linear;
        ptr->quadratic = quadratic;
        return ptr;
    }

    float calcRadius();
};

#endif // ENGINE_GRAPHICS_COMPONENTS_POINTLIGHT_H_
