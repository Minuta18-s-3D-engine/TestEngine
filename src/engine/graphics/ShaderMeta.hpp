#ifndef ENGINE_GRAPHICS_SHADERMETA_HPP
#define ENGINE_GRAPHICS_SHADERMETA_HPP

#include <string>
#include <vector>

#include "ShaderPropertyTypes.hpp"
#include "engine/resource/serialization/Serialization.hpp"

struct ShaderMeta {
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

    struct Sources {
        std::string vertexPath;
        std::string fragmentPath;
        std::string computePath;

        GENERATE_SERIALIZATION(Sources,
            SERIALIZE_FIELD_DEFAULT(vertexPath, std::string()),
            SERIALIZE_FIELD_DEFAULT(fragmentPath, std::string()),
            SERIALIZE_FIELD_DEFAULT(computePath, std::string())
        )
    };

    struct CodeGeneration {
        std::variant<bool, std::string> generateUnpack = "default";

        GENERATE_SERIALIZATION(CodeGeneration,
            SERIALIZE_FIELD_DEFAULT(generateUnpack,
                std::variant<bool, std::string>("default"))
        )
    };

    Sources sources;
    std::vector<PropertyEntry> properties;
    std::vector<SamplerEntry> samplers;
    CodeGeneration codeGeneration;

    GENERATE_SERIALIZATION(ShaderMeta,
        SERIALIZE_FIELD(sources),
        SERIALIZE_FIELD_DEFAULT(properties, std::vector<PropertyEntry>());
        SERIALIZE_FIELD_DEFAULT(samplers, std::vector<SamplerEntry>()),
        SERIALIZE_FIELD_DEFAULT(codeGeneration, CodeGeneration())
    )
};

#endif // ENGINE_GRAPHICS_SHADERMETA_HPP
