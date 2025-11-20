#ifndef ENGINE_GRAPHICS_LIGHTBVHWRAPPER_H_
#define ENGINE_GRAPHICS_LIGHTBVHWRAPPER_H_

#include <bvh/v2/bvh.h>
#include <bvh/v2/vec.h>
#include <bvh/v2/ray.h>
#include <bvh/v2/node.h>
#include <bvh/v2/default_builder.h>
#include <bvh/v2/thread_pool.h>
#include <bvh/v2/executor.h>
#include <bvh/v2/stack.h>
#include <bvh/v2/tri.h>

#include <vector>
#include <memory>

#include "../materials/Light.hpp"

struct GPUBVHNode {
    glm::vec4 minBounds;
    glm::vec4 maxBounds;
    uint32_t leftChild;
    uint32_t rightChild;
    uint32_t lightStart;
    uint32_t lightCount;
};

class LightBVHWrapper {
public:
    using Scalar  = float;
    using Vec3    = bvh::v2::Vec<Scalar, 3>;
    using BBox    = bvh::v2::BBox<Scalar, 3>;
    using Node    = bvh::v2::Node<Scalar, 3>;
    using Bvh     = bvh::v2::Bvh<Node>;
    using Config  = bvh::v2::DefaultBuilder<Node>::Config;
private:
    using LightArray = std::vector<std::shared_ptr<Light>>;

    std::vector<GPUBVHNode> gpuNodes;
    std::vector<uint32_t> gpuLightIndices;

    std::unique_ptr<Bvh> bvhTree;
    std::unique_ptr<Config> bvhConfig;

    void initBVH(const LightArray& lights);
public:
    LightBVHWrapper() = delete; // yet
    LightBVHWrapper(const LightArray& lights);

    std::vector<LightBVHWrapper::Node>& getGpuNodes();
    std::vector<size_t>& getGpuLightIndicies();
};

#endif // ENGINE_GRAPHICS_LIGHTBVHWRAPPER_H_
