#ifndef TESTENG_SHADERIMPORTER_H
#define TESTENG_SHADERIMPORTER_H

#include "engine/resource/ResourceImporter.hpp"
#include "engine/debug/logging/Logging.hpp"

class MaterialLayout;
class Shader;
struct ShaderSchema;

class ShaderImporter : public ResourceImporter<Shader> {
    Logger logger;

    [[nodiscard]] Shader loadShader(
        const VirtualPath& path, ResourceManager& resourceManager
    );
    void addPropertyToLayout(
        MaterialLayout& layout,
        const std::string& propName,
        const std::string& propType
    );
    [[nodiscard]] MaterialLayout createLayout(const ShaderSchema& schema);
    std::string readSourceFile(const VirtualPath& path) const;
public:
    explicit ShaderImporter(Serializer& serializer_);
    ~ShaderImporter() override = default;

    [[nodiscard]] Shader import(
        const VirtualPath& path, ResourceManager& resourceManager
    ) override;
};

#endif //TESTENG_SHADERIMPORTER_H
