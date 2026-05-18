#ifndef ENGINE_GRAPHICS_SAMPLERDEFINITION_H_
#define ENGINE_GRAPHICS_SAMPLERDEFINITION_H_

#include <string>
#include <stdint.h>

#include "SamplerType.hpp"

struct SamplerDefinition {
    std::string name;
    SamplerType type;
    uint32_t slot;
};

#endif // ENGINE_GRAPHICS_SAMPLERDEFINITION_H_
