#include "MaterialRegistry.hpp"

MaterialRegistry::MaterialRegistry() : 
    gpuInstancesData(SSBOBindings::MATERIALS, GL_DYNAMIC_DRAW) {
    cpuInstancesData.resize(GROW_STEP, 0);
}

void MaterialRegistry::increaseSize(size_t newOffset) {
    if (newOffset <= cpuInstancesData.size()) {
        return;
    }
    size_t newAllocatedData = (newOffset + GROW_STEP - 1) / 
        GROW_STEP * GROW_STEP;
    cpuInstancesData.resize(newAllocatedData, 0);
    currentOffset = newOffset;
}

MaterialRegistry::MaterialInstanceID 
MaterialRegistry::registerMaterialInstance(
    std::shared_ptr<MaterialInstance> instance
) {
    std::shared_ptr<Material> material = instance->getMaterial();
    const auto& properties = material->getPropertyStorage();

    size_t propertiesSize = properties.getDataSize();
    size_t propertiesAlignment = properties.getDataAlignment();

    currentOffset = (currentOffset + propertiesAlignment - 1) & 
        ~(propertiesAlignment - 1);
    increaseSize(currentOffset + propertiesSize);
    
    size_t prevCurrentOffset = currentOffset;
    currentOffset += propertiesSize;

    MaterialInstanceID id = instanceMetadata.size();
    instanceMetadata.push_back({
        .offset = prevCurrentOffset,
        .size = propertiesSize,
        .materialInstance = instance
    });

    std::memcpy(
        &cpuInstancesData[prevCurrentOffset],
        properties.getRawData(),
        propertiesSize
    );

    return id;
}
