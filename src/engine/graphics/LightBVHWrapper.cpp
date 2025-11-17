#include "LightBVHWrapper.hpp"

void LightBVHWrapper::initBVH(const LightBVHWrapper::LightArray& lights) {
    std::vector<BBox> bboxes(lights.size());
    std::vector<Vec3> centers(lights.size());

    bvh::v2::ThreadPool thread_pool;
    bvh::v2::ParallelExecutor executor(thread_pool);

    executor.for_each(0, lights.size(), [&] (size_t begin, size_t end) {
        for (size_t i = begin; i < end; ++i) {
            auto light = lights[i]; 
            Vec3 center = Vec3(light->position.x, light->position.y, light->position.z);
            Vec3 radius = Vec3(light->calcRadius());

            BBox lightBBox(center - radius, center + radius);
            bboxes[i] = lightBBox;
            centers[i] = center;
        }
    });

    bvhConfig = new Config();
    bvhConfig->quality = bvh::v2::DefaultBuilder<Node>::Quality::High;
    bvhTree = bvh::v2::DefaultBuilder<Node>::build(
        thread_pool, bboxes, centers, bvhConfig
    );
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
