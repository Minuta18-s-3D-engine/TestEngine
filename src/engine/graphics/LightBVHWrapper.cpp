#include "LightBVHWrapper.hpp"

void LightBVHWrapper::initBVH(const LightBVHWrapper::LightArray& lights) {
    std::vector<BBox> bboxes;
    std::vector<Vec3> centers;

    bboxes.reserve(lights.size());
    centers.reserve(lights.size());

    bvh::v2::ThreadPool thread_pool;
    bvh::v2::ParallelExecutor executor(thread_pool);

    executor.for_each(0, lights.size(), [&] (size_t begin, size_t end) {
        for (size_t i = begin; i < end; ++i) {
            Vec3 center = Vec3(light->position.x, light->position.y, light->position.z);
            Vec3 radius = Vec3(light->calcRadius());

            BBox lightBBox(center - radius, center + radius);
            bboxes.push_back(lightBBox);
            centers.push_back(center);
        }
    });

    bvhConfig = new Config();
    bvhConfig->quality = bvh::v2::DefaultBuilder<Node>::Quality::High;
    bvh = &(bvh::v2::DefaultBuilder<Node>::build(
        thread_pool,
        bboxes,
        centers,
        bvhConfig
    ));
}

LightBVHWrapper::LightBVHWrapper() {

}

LightBVHWrapper::LightBVHWrapper(const LightArray& lights) {

}

LightBVHWrapper::~LightBVHWrapper() {
    delete bvh;
    delete bvhConfig;
}

std::vector<GPUBVHNode>& LightBVHWrapper::getGpuNodes() {

}

std::vector<uint32_t> LightBVHWrapper::getGpuLightIndicies() {

}
