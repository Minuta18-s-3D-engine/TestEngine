#include "LightBVHWrapper.hpp"

void LightBVHWrapper::initBVH(const LightBVHWrapper::LightArray& lights) {
    std::vector<BBox> bboxes(lights.size());
    std::vector<Vec3> centers(lights.size());

    bvh::v2::ThreadPool thread_pool;
    bvh::v2::ParallelExecutor executor(thread_pool);

    executor.for_each(0, lights.size(), [&] (size_t begin, size_t end) {
        for (size_t i = begin; i < end; ++i) {
            auto light = lights[i]; 
            auto transformComponent = light->getComponent<Transform>();
            auto lightComponent = light->getComponent<PointLight>();
            Vec3 center = Vec3(
                transformComponent->position.x, 
                transformComponent->position.y, 
                transformComponent->position.z
            );
            Vec3 radius = Vec3(lightComponent->calcRadius());
            // constexpr double lightRadius = 0.01;
            // Vec3 radius = Vec3(lightRadius, lightRadius, lightRadius);

            BBox lightBBox(center - radius, center + radius);
            bboxes[i] = lightBBox;
            centers[i] = center;
        }
    });

    bvhConfig = std::make_unique<Config>();
    bvhConfig->quality = bvh::v2::DefaultBuilder<Node>::Quality::High;
    bvhTree = std::make_unique<Bvh>(bvh::v2::DefaultBuilder<Node>::build(
        thread_pool, bboxes, centers, *bvhConfig.get()
    ));
}

LightBVHWrapper::LightBVHWrapper(const LightArray& lights) {
    this->initBVH(lights);
}

std::vector<LightBVHWrapper::Node>& LightBVHWrapper::getGpuNodes() {
    return bvhTree->nodes;
}

std::vector<size_t>& LightBVHWrapper::getGpuLightIndicies() {
    return bvhTree->prim_ids;
}
