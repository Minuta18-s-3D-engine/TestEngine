#ifndef ENGINE_MATERIALS_MATERIALDESCRIPTOR_H_
#define ENGINE_MATERIALS_MATERIALDESCRIPTOR_H_

#include <string>
#include <vector>
#include <unordered_map>

#include "MaterialGraphicsConfig.hpp"
#include "MaterialLayout.hpp"
#include "engine/graphics/SamplerDefinition.hpp"
#include "engine/graphics/Texture.hpp"
#include "engine/resource/ResourceManager.hpp"

struct MaterialDescriptor {
    using SamplerMap = std::unordered_map<
        std::string, ResourceHandle<Texture>>;
    using SamplerIndexesMap = std::unordered_map<std::string, size_t>;

    std::string name;
    MaterialGraphicsConfig config;
    MaterialLayout layout;

    std::vector<SamplerDefinition> samplerDefinitions;
    SamplerIndexesMap samplerIndexes;
};

#endif // ENGINE_MATERIALS_MATERIALDESCRIPTOR_H_
