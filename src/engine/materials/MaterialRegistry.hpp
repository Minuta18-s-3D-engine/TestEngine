#ifndef ENGINE_MATERIALS_MATERIALREGISTRY_H_
#define ENGINE_MATERIALS_MATERIALREGISTRY_H_

#include "Material.hpp"
#include "../graphics/ShaderStorageBuffer.hpp"
#include "MaterialInstance.hpp"

constexpr size_t B = 1;
constexpr size_t KB = 1024 * B;
constexpr size_t MB = 1024 * KB;

class MaterialRegistry {
public:
    using MaterialID = size_t;
    using MaterialInstanceID = size_t;

    const size_t GROW_STEP = 2 * MB;

    struct MaterialInstanceMetadata {
        size_t offset;
        size_t size;
        std::shared_ptr<MaterialInstance> materialInstance;
    };
private:
    std::vector<uint8_t> cpuInstancesData;
    size_t currentOffset = 0;

    ShaderStorageBuffer gpuInstancesData;
    std::vector<MaterialInstanceMetadata> instanceMetadata;

    void increaseSize(size_t newOffset);
public:
    MaterialRegistry();

    MaterialInstanceID registerMaterialInstance(
        std::shared_ptr<MaterialInstance> instance
    );
};

#endif // ENGINE_MATERIALS_MATERIALREGISTRY_H_ 
