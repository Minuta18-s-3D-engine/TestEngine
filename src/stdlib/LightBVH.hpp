#ifndef ENGINE_STDLIB_LIGHTBVH_H_
#define ENGINE_STDLIB_LIGHTBVH_H_

#include <memory>
#include <vector>

struct CompLight;

class LightBVH {
public:
    LightBVH();
    LightBVH(std::vector<CompLight>& lights);
    ~LightBVH();

    void insert(CompLight& light);
};

#endif // ENGINE_STDLIB_LIGHTBVH_H_
