#include "PointLight.hpp"

const float DEFAULT_LINEAR = 0.04;
const float DEFAULT_QUADRATIC = 0.01;
const glm::vec3 DEFAULT_VEC3_ARG = {1.0f, 1.0f, 1.0f};

PointLight::PointLight() : color(DEFAULT_VEC3_ARG), 
    linear(DEFAULT_LINEAR), quadratic(DEFAULT_QUADRATIC) {}
PointLight::PointLight(
    glm::vec3 _color,
    float _linear,
    float _quadratic
) : color(_color), 
    linear(_linear), quadratic(_quadratic) {}
PointLight::PointLight(
    glm::vec3 _color 
) : color(_color), 
    linear(DEFAULT_LINEAR), quadratic(DEFAULT_QUADRATIC) {}

float PointLight::calcRadius() {
    // idk where that numbers came from
    float maxBrightness = std::fmaxf(std::fmaxf(color.r, color.g), color.b);
    float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * 
        (1.0 - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
    
    return radius;
}
