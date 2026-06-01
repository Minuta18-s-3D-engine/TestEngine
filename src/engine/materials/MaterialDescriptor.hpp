#ifndef ENGINE_MATERIALS_MATERIALDESCRIPTOR_H_
#define ENGINE_MATERIALS_MATERIALDESCRIPTOR_H_

#include <string>
#include <vector>

#include "MaterialGraphicsConfig.hpp"
#include "MaterialLayout.hpp"
#include "../graphics/SamplerDefinition.hpp"
#include "../graphics/Texture.hpp"

struct MaterialDescriptor {
    using SamplerMap = std::unordered_map<
        std::string, std::shared_ptr<Texture>>;
    using SamplerIndexesMap = std::unordered_map<std::string, size_t>;

    std::string name;
    MaterialGraphicsConfig config;
    MaterialLayout layout;

    std::vector<SamplerDefinition> samplerDefinitions;
    SamplerIndexesMap samplerIndexes;
};

#endif // ENGINE_MATERIALS_MATERIALDESCRIPTOR_H_
