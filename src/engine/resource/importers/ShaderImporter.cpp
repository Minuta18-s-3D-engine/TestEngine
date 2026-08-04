#include "ShaderImporter.hpp"

#include "engine/graphics/Shader.hpp"
#include "engine/graphics/ShaderSchema.h"
#include "engine/materials/MaterialLayout.hpp"
#include "engine/materials/templateGenerators/ShaderCodeGenerator.hpp"

ShaderImporter::ShaderImporter(Serializer& serializer_)
    : ResourceImporter(serializer_),
      logger(Logging::createLogger(
      "engine.resource.importers.shaderImporter")) {}

void ShaderImporter::addPropertyToLayout(
    MaterialLayout& layout,
    const std::string& propName,
    const std::string& propType
) {
    #define X(name, type, glslType) \
        if (propType == glslType) \
            layout.addProperty<type>(propName);

        PROPERTY_TYPE_LIST
    #undef X
}

MaterialLayout ShaderImporter::createLayout(const ShaderSchema& schema) {
    MaterialLayout layout;

    for (const auto& [propName, typeStr] : schema.layout.properties) {
        addPropertyToLayout(layout, propName, typeStr);
    }

    for (const auto& [samplerName, typeStr] : schema.layout.samplers) {
        layout.addProperty<glm::uvec2>(samplerName);
    }

    layout.finalize();
    return layout;
}

std::string ShaderImporter::readSourceFile(const VirtualPath& path) const {
    File sourceFile(path);
    return sourceFile.readAll();
}

Shader ShaderImporter::loadShader(
    const VirtualPath& path, ResourceManager& resourceManager
) {
    logger.info("Loading shader: {}...", path.resolve());

    StructuredData::DataNode metaContents = parseMetaFile(path);
    ShaderSchema schema;
    StructuredData::Mapper::read<ShaderSchema>(metaContents, schema);

    MaterialLayout shaderLayout = createLayout(schema);

    ShaderSources sources;
    ShaderCodeGenerator generator;

    if (!schema.files.compute.empty()) {
        std::string rawCompute = readSourceFile(schema.files.compute);
        sources.compute = generator.generateCompShader(
            rawCompute, "compute"
        );
    }
    if (!schema.files.vertex.empty()) {
        std::string rawVertex = readSourceFile(schema.files.vertex);
        sources.vertex = generator.generateShader(
            shaderLayout, rawVertex, "vertex" // TODO: ShaderCodeGenerator update
        );
    }
    if (!schema.files.fragment.empty()) {
        std::string rawFragment = readSourceFile(schema.files.fragment);
        sources.fragment = generator.generateShader(
            shaderLayout, rawFragment, "fragment"
        );
    }

    return Shader(sources);
}

Shader ShaderImporter::import(
    const VirtualPath &path, ResourceManager &resourceManager
) {
    try {
        return loadShader(path, resourceManager);
    } catch (exc::importer_exceptions::importer_exception& e) {
        throw;
    } catch (std::exception& e) {
        throw exc::importer_exceptions::importer_exception(e.what());
    }
}
