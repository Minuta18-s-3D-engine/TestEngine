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
    const VirtualPath& path, const ShaderLayout& layout,
    const ShaderMeta& meta, const std::string& callFunc
) const {
    File shaderFile(path);
    std::string source = shaderFile.readAll();

    bool generateUnpack = true;
    if (std::holds_alternative<std::string>(meta.codeGeneration.generateUnpack)) {
        if (callFunc == "compute") {
            generateUnpack = false;
        }
    } else {
        generateUnpack = std::get<bool>(meta.codeGeneration.generateUnpack);
    }

    std::string fullSource = generator.generateShader(
        layout, source, callFunc, generateUnpack
    );

    return fullSource;
}

ShaderMeta::ShaderSources ShaderImporter::getShaderSources(
    const ShaderLayout& layout, const ShaderMeta& meta
) const {
    ShaderMeta::ShaderSources sources;

    for (const auto& [type, shaderPath] : meta.sourcesPaths) {
        File shaderFile(shaderPath);
        const std::string generatedSource = generateShaderSource(
            shaderPath, layout, meta, "..."
        );
        sources[type] = generatedSource;
    }

    return sources;
}

Shader ShaderImporter::loadShader(const VirtualPath& path) {
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

    ShaderMeta::ShaderSources sources = getShaderSources(layout, meta);
    Shader createdShader(sources, std::move(layout));

    logger.info("Shader {} loaded successfully", path.resolve());

    return createdShader;
}

Shader ShaderImporter::import(
    const VirtualPath& path, ResourceManager& resourceManager
) {
    try {
        return loadShader(path);
    } catch (exc::importer_exceptions::importer_exception& e) {
        throw;
    } catch (std::exception& e) {
        throw exc::importer_exceptions::importer_exception(e.what());
    }
}
