#ifndef ENGINE_MATERIALS_MATERIALDATABUFFER_H_
#define ENGINE_MATERIALS_MATERIALDATABUFFER_H_

#include <stdint.h>
#include <vector>

#include "../graphics/ShaderStorageBuffer.hpp"

struct alignas(16) MaterialInstanceShaderMetadata {
    uint32_t offset;
    uint32_t size;
};

class MaterialDataBuffer {
    std::vector<uint8_t> cpuBuffer;    
    std::vector<MaterialInstanceShaderMetadata> instances;

    ShaderStorageBuffer gpuDataBuffer;
    ShaderStorageBuffer gpuMetaBuffer;
public:
    

    void sync();
    void bind() const;
};

#endif // ENGINE_MATERIALS_MATERIALDATABUFFER_H_

