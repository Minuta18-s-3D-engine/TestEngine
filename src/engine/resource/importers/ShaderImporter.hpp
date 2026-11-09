#ifndef TESTENG_SHADERIMPORTER_H
#define TESTENG_SHADERIMPORTER_H

#include "engine/resource/ResourceImporter.hpp"
#include "engine/debug/logging/Logging.hpp"
#include "engine/graphics/ShaderLayout.hpp"
#include "engine/graphics/ShaderMeta.hpp"
#include "engine/materials/templateGenerators/ShaderCodeGenerator.hpp"

class MaterialLayout;
class Shader;
struct ShaderSchema;

class ShaderImporter : public ResourceImporter<Shader> {
    Logger logger;

    ResourceManager* manager;
    ShaderCodeGenerator generator;

    std::string generateShaderSource(
        const VirtualPath& path, const ShaderLayout& layout,
        const ShaderMeta& meta, const std::string& callFunc
    );
    Shader loadShader(const VirtualPath& path, ResourceManager& manager);
public:
    explicit ShaderImporter(Serializer& serializer_, ResourceManager& manager_);

    [[nodiscard]] Shader import(
        const VirtualPath& path, ResourceManager& resourceManager
    ) override;
};

#endif //TESTENG_SHADERIMPORTER_H
