#ifndef TESTENG_SHADERSCHEMA_H
#define TESTENG_SHADERSCHEMA_H

#include <tuple>
#include <string>
#include <unordered_map>

#include "engine/containers/reflection/Reflection.hpp"

struct ShaderSchema {
    enum class RenderingType {
        Deferred,
        Forward
    };

    struct Files {
        std::string vertex;
        std::string fragment;
        std::string compute;
    };

    struct Config {
        RenderingType renderingType = RenderingType::Deferred;
    };

    struct Layout {
        std::unordered_map<std::string, std::string> properties;
        std::unordered_map<std::string, std::string> samplers;
    };

    Files files;
    Config config;
    Layout layout;
};

ENUM_SCHEMA(ShaderSchema::RenderingType,
    Reflection::EnumField("deferred", ShaderSchema::RenderingType::Deferred),
    Reflection::EnumField("forward", ShaderSchema::RenderingType::Forward)
)

STRUCT_SCHEMA(ShaderSchema::Files,
    Reflection::Field("vertex", &ShaderSchema::Files::vertex,
        std::string("")),
    Reflection::Field("fragment", &ShaderSchema::Files::fragment,
        std::string("")),
    Reflection::Field("compute", &ShaderSchema::Files::compute,
        std::string(""))
)

STRUCT_SCHEMA(ShaderSchema::Config,
    Reflection::Field("renderingType", &ShaderSchema::Config::renderingType,
        ShaderSchema::RenderingType::Deferred)
)

STRUCT_SCHEMA(ShaderSchema::Layout,
    Reflection::Field("properties", &ShaderSchema::Layout::properties,
        std::unordered_map<std::string, std::string>{}),
    Reflection::Field("samplers", &ShaderSchema::Layout::samplers,
        std::unordered_map<std::string, std::string>{})
)

STRUCT_SCHEMA(ShaderSchema,
    Reflection::Field("files", &ShaderSchema::files),
    Reflection::Field("config", &ShaderSchema::config, ShaderSchema::Config{}),
    Reflection::Field("layout", &ShaderSchema::layout, ShaderSchema::Layout{})
)

#endif //TESTENG_SHADERSCHEMA_H
