#include "MaterialDataBuffer.hpp"

uint32_t MaterialDataBuffer::allocateBlock(uint32_t size) {
    uint32_t offset = (cpuBuffer.size() + blockAlignment - 1) &
        ~(blockAlignment - 1);

    cpuBuffer.resize(offset + size, 0);

    uint32_t id = instances.size();
    instances.push_back({ offset, size });

    return id;
}

void MaterialDataBuffer::write(
    uint32_t id, uint32_t offset, uint32_t size, const void* data
) {
    const auto& meta = instances.at(id);
    std::memcpy(cpuBuffer.data() + meta.offset + offset, data, size);
}

void MaterialDataBuffer::read(
    uint32_t id, uint32_t offset, uint32_t size, void* outData
) {
    const auto& meta = instances.at(id);
    std::memcpy(outData, cpuBuffer.data() + meta.offset + offset, size);
}

void MaterialDataBuffer::sync() {
    uint32_t metaBufferSize = instances.size() * 
        sizeof(MaterialInstanceShaderMetadata);
    
    if (lastDataSize < cpuBuffer.size()) {
        gpuDataBuffer.setData(cpuBuffer.data(), cpuBuffer.size());
        lastDataSize = cpuBuffer.size();
    } else {
        gpuDataBuffer.setSubData(cpuBuffer.data(), 0, cpuBuffer.size());
    }

    if (lastMetaSize < instances.size()) {
        gpuMetaBuffer.setData(instances.data(), metaBufferSize);
        lastMetaSize = instances.size();
    } else {
        gpuMetaBuffer.setSubData(instances.data(), 0, metaBufferSize);
    }
}

void MaterialDataBuffer::bind() {
    gpuDataBuffer.bind();
    gpuMetaBuffer.bind();
}

const MaterialInstanceShaderMetadata& MaterialDataBuffer::getMetadataById(
    uint32_t id
) const {
    return instances.at(id);
}
