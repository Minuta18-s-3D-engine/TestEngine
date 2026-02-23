#include "MaterialRegistry.hpp"

MaterialRegistry::MaterialRegistry() : 
    gpuInstancesData(SSBOBindings::MATERIALS, GL_DYNAMIC_DRAW),
    gpuInstancesMetadata(SSBOBindings::MATERIAL_INDICIES, GL_DYNAMIC_DRAW) {
    cpuInstancesData.resize(GROW_STEP, 0);
}

void MaterialRegistry::markDirty(size_t offset, size_t size) {
    dirtyStart = std::min(dirtyStart, offset);
    dirtyEnd = std::max(dirtyEnd, offset + size);
}

void MaterialRegistry::increaseSize(size_t newOffset) {
    if (newOffset <= cpuInstancesData.size()) {
        return;
    }
    size_t newAllocatedData = (newOffset + GROW_STEP - 1) / 
        GROW_STEP * GROW_STEP;
    cpuInstancesData.resize(newAllocatedData, 0);
    gpuInstancesData.setData(cpuInstancesData.data(), newAllocatedData);
    markDirty(0, newAllocatedData);
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

    MaterialInstanceShaderMetadata shaderMetadata = {
        .offset = static_cast<uint32_t>(prevCurrentOffset),
        .size = static_cast<uint32_t>(propertiesSize)
    };
    instanceShaderMetadata.push_back(shaderMetadata);
    instanceShaderMetadataDirty = true;

    std::memcpy(
        &cpuInstancesData[prevCurrentOffset],
        properties.getRawData(),
        propertiesSize
    );
    markDirty(prevCurrentOffset, propertiesSize);

    return id;
}

void MaterialRegistry::updateMaterialInstanceData(
    MaterialInstanceID id
) {
    if (id >= instanceMetadata.size()) {
        throw std::out_of_range("Invalid MaterialInstanceID.");
    }

    std::memcpy(
        &cpuInstancesData[instanceMetadata[id].offset],
        instanceMetadata[id].materialInstance->getPropertyStorage()
            .getRawData(),
        instanceMetadata[id].size
    );
    markDirty(instanceMetadata[id].offset, instanceMetadata[id].size);
}

void MaterialRegistry::syncGPU() {
    if (dirtyStart >= dirtyEnd) return;
     
    gpuInstancesData.setSubData(
        &cpuInstancesData[dirtyStart],
        dirtyStart, 
        dirtyEnd - dirtyStart
    );

    dirtyStart = std::numeric_limits<size_t>::max();
    dirtyEnd = 0;

    if (instanceShaderMetadataDirty) {
        gpuInstancesMetadata.setData(
            instanceShaderMetadata.data(),
            instanceShaderMetadata.size() * sizeof(MaterialInstanceShaderMetadata)
        );
        instanceShaderMetadataDirty = false;
    }
}
