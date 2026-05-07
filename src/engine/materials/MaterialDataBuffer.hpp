#ifndef ENGINE_MATERIALS_MATERIALDATABUFFER_H_
#define ENGINE_MATERIALS_MATERIALDATABUFFER_H_

#include <stdint.h>
#include <vector>
#include <cstring>

#include "../graphics/ShaderStorageBuffer.hpp"

struct alignas(16) MaterialInstanceShaderMetadata {
    uint32_t offset;
    uint32_t size;
};

class MaterialDataBuffer {
    const uint32_t blockAlignment = 16;

    std::vector<uint8_t> cpuBuffer;    
    std::vector<MaterialInstanceShaderMetadata> instances;

    ShaderStorageBuffer gpuDataBuffer{
        SSBOBindings::MATERIALS, GL_DYNAMIC_DRAW};
    ShaderStorageBuffer gpuMetaBuffer{
        SSBOBindings::MATERIALS_META, GL_DYNAMIC_DRAW};

    uint32_t lastDataSize = 0, lastMetaSize = 0;
public:
    uint32_t allocateBlock(uint32_t size);

    void write(uint32_t id, uint32_t offset, uint32_t size, const void* data);
    void read(uint32_t id, uint32_t offset, uint32_t size, void* outData);

    void sync();
    void bind();
};

#endif // ENGINE_MATERIALS_MATERIALDATABUFFER_H_

