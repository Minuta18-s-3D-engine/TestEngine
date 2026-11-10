#ifndef ENGINE_GRAPHICS_SHADERMETA_HPP
#define ENGINE_GRAPHICS_SHADERMETA_HPP

#include <string>
#include <vector>

#include "ShaderPropertyTypes.hpp"
#include "engine/resource/serialization/Serialization.hpp"

struct ShaderMeta {
    enum class ShaderType {
        VERTEX,
        FRAGMENT,
        COMPUTE
    };

    struct PropertyEntry {
        std::string name;
        shader_layout::PropertyType type;

        GENERATE_SERIALIZATION(PropertyEntry,
            SERIALIZE_FIELD(name),
            SERIALIZE_FIELD(type)
        )
    };

    struct SamplerEntry {
        std::string name;
        shader_layout::SamplerType type;

        GENERATE_SERIALIZATION(SamplerEntry,
            SERIALIZE_FIELD(name),
            SERIALIZE_FIELD(type)
        )
    };

    struct CodeGeneration {
        std::variant<bool, std::string> generateUnpack = "default";

        GENERATE_SERIALIZATION(CodeGeneration,
            SERIALIZE_FIELD_DEFAULT(generateUnpack,
                std::variant<bool, std::string>("default"))
        )
    };

    using ShaderSources = std::map<ShaderType, std::string>;

    ShaderSources sourcesPaths;
    std::vector<PropertyEntry> properties;
    std::vector<SamplerEntry> samplers;
    CodeGeneration codeGeneration;

    GENERATE_SERIALIZATION(ShaderMeta,
        SERIALIZE_FIELD(sourcesPaths),
        SERIALIZE_FIELD_DEFAULT(properties, std::vector<PropertyEntry>());
        SERIALIZE_FIELD_DEFAULT(samplers, std::vector<SamplerEntry>()),
        SERIALIZE_FIELD_DEFAULT(codeGeneration, CodeGeneration())
    )
};

GENERATE_ENUM_SERIALIZATION(ShaderMeta::ShaderType,
    ENUM_OPTION(ShaderMeta::ShaderType::VERTEX, "vertex"),
    ENUM_OPTION(ShaderMeta::ShaderType::FRAGMENT, "fragment"),
    ENUM_OPTION(ShaderMeta::ShaderType::COMPUTE, "compute")
)

#endif // ENGINE_GRAPHICS_SHADERMETA_HPP
