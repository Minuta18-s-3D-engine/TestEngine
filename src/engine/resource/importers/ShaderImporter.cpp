#include "ShaderImporter.hpp"

#include "engine/graphics/Shader.hpp"
#include "engine/materials/templateGenerators/ShaderCodeGenerator.hpp"
#include "engine/resource/serialization/ObjectSerializer.hpp"

ShaderImporter::ShaderImporter(
    Serializer& serializer_, ResourceManager& manager_
) : ResourceImporter(serializer_),
    logger(Logging::createLogger(
        "engine.resource.importers.shaderImporter")),
    manager(&manager_),
    generator(manager_, ShaderCodeGenerator::FormattingOptions()) {}

std::string ShaderImporter::generateShaderSource(
    const VirtualPath &path, const ShaderLayout &layout,
    const ShaderMeta &meta, const std::string &callFunc
) {
    File shaderFile(path);
    std::string source = shaderFile.readAll();

    bool generateUnpack = true;
    if (std::holds_alternative<std::string>(meta.codeGeneration.generateUnpack)) {
        // WIP
    }

    std::string fullSource = generator.generateShader(
        layout, source, callFunc,
    );
}

Shader ShaderImporter::loadShader(
    const VirtualPath &path, ResourceManager &manager
) {
    logger.info("Loading shader: {}...", path.resolve());

    StructuredData::DataNode metaContents = parseMetaFile(path);
    ShaderMeta meta;
    ObjectSerializer::deserialize(metaContents, meta);

    ShaderLayout layout;
    for (const auto& prop : meta.properties) {
        layout.addProperty(prop.name, prop.type);
    }
    for (const auto& sampler : meta.samplers) {
        layout.addSampler(sampler.name, sampler.type);
    }
    layout.finalize();

    ShaderSources sources;
    if (!meta.sources.fragmentPath.empty()) {
        File fragmentFile(path);
        sources.fragment = fragmentFile.readAll();
    }
    if (!meta.sources.vertexPath.empty()) {
        File vertexFile(path);
        sources.vertex = vertexFile.readAll();
    }
    if (!meta.sources.computePath.empty()) {
        File computeFile(path);
        sources.compute = computeFile.readAll();
    }

    logger.info("Shader {} loaded successfully", path.resolve());

    return Shader(sources, std::move(layout));
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
