#include "LightBVHWrapper.hpp"

void LightBVHWrapper::build(const LightBVHWrapper::LightArray& lights) {
    std::vector<BBox> bboxes;
    std::vector<Vec3> centers;

    bboxes.reserve(lights.size());
    centers.reserve(lights.size());

    for (auto light : lights) {
        Vec3 center = Vec3(light->position.x, light->position.y, light->position.z);
        Vec3 radius = Vec3(light->calcRadius());

        BBox lightBBox(center - radius, center + radius);
        bboxes.push_back(lightBBox);
        centers.push_back(center);
    }

    // bvh = new 
}
