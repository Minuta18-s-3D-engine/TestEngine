#ifndef ENGINE_STDLIB_LIGHTBVH_H_
#define ENGINE_STDLIB_LIGHTBVH_H_

#include "glm/glm.hpp"

#include <memory>
#include <vector>

struct CompLight;

struct alignas(16) LightBVHNode {
    alignas(16) glm::vec3 AABBp1;
    alignas(16) glm::vec3 AABBp2;
    uint leftChildIndex;
    uint rightChildIndex;
    uint lightIndex;
    bool isLeaf;
};

class LightBVH {
public:
    LightBVH();
    LightBVH(std::vector<CompLight>& lights);
    ~LightBVH();

    void insert(CompLight& light);
};

#endif // ENGINE_STDLIB_LIGHTBVH_H_
