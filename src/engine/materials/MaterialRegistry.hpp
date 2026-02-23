#ifndef ENGINE_MATERIALS_MATERIALREGISTRY_H_
#define ENGINE_MATERIALS_MATERIALREGISTRY_H_

#include <vector>
#include <memory>

#include "Material.hpp"
#include "../graphics/ShaderStorageBuffer.hpp"
#include "MaterialInstance.hpp"

constexpr size_t B = 1;
constexpr size_t KB = 1024 * B;
constexpr size_t MB = 1024 * KB;

struct alignas(16) MaterialInstanceShaderMetadata {
    uint32_t offset;
    uint32_t size;
};

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
    ShaderStorageBuffer gpuInstancesMetadata;
    std::vector<MaterialInstanceMetadata> instanceMetadata;
    std::vector<MaterialInstanceShaderMetadata> instanceShaderMetadata;
    bool instanceShaderMetadataDirty = false;

    size_t dirtyStart = std::numeric_limits<size_t>::max(), dirtyEnd = 0;

    void markDirty(size_t offset, size_t size);
    void increaseSize(size_t newOffset);
public:
    MaterialRegistry();

    MaterialInstanceID registerMaterialInstance(
        std::shared_ptr<MaterialInstance> instance
    );

    void updateMaterialInstanceData(
        MaterialInstanceID id
    );

    void syncGPU();
};

#endif // ENGINE_MATERIALS_MATERIALREGISTRY_H_ 
