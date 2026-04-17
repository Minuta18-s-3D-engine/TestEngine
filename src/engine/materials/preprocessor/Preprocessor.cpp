#include "Preprocessor.hpp"

Preprocessor::Preprocessor(
    FilesystemAbstraction& _filesystem
) : filesystem(_filesystem), 
    cache(_filesystem, "fs://.cache/materialCode") {}

PreprocessorCache::ProcessedShader Preprocessor::parseOrLoad(
    const VirtualPath& filePath
) {
    {
        auto processedShader = cache.load(filePath);
        if (processedShader.has_value()) {
            return processedShader.value();
        }
    }

    std::string contents = filesystem.readFile(filePath);
    auto parser = createParser(contents);
    auto result = parser->parse();

    std::vector<VirtualPath> dependencies;
    for (auto& directive : result.directives) {
        if (!directive.nameMatch({ IMPORT_DIRECTIVE })) continue;

        const size_t argIndex = 0;
        if (directive.args.size() < argIndex + 1) continue;
        if (directive.args.at(0).type == PreprocessorParser::ArgType::String)
            continue;

        dependencies.push_back(std::string(directive.args.at(argIndex).value));
    }

    PreprocessorCache::ProcessedShader cacheEntry(
        filePath,
        result.code,
        dependencies
    );

    cache.store(cacheEntry);

    return cacheEntry;
}

void Preprocessor::buildDependencyGraph(
    const VirtualPath& filePath
) {
    ShaderDependencyGraph::NodeId nodeId(filePath);

    if (dependencyGraph.nodeExists(nodeId)) return;

    PreprocessorCache::ProcessedShader parsedShader = parseOrLoad(
        filePath
    );

    for (auto& path : parsedShader.dependencies) {
        buildDependencyGraph(path);
        ShaderDependencyGraph::NodeId depNodeId(path);
        dependencyGraph.addDependency(nodeId, depNodeId);
    }
}

std::shared_ptr<PreprocessorParser> Preprocessor::createParser(
    const std::string& fileContents
) {
    auto parser = std::make_shared<PreprocessorParser>(fileContents);

    using ArgType = PreprocessorParser::ArgType;
    parser->addDirectiveValidator(
        IMPORT_DIRECTIVE, { ArgType::String }
    );

    return parser;
}

std::string Preprocessor::preprocess(const VirtualPath& filePath) {
    buildDependencyGraph(filePath);

    ShaderDependencyGraph::NodeId rootNode(filePath);
    auto sorted = dependencyGraph.getSortedDependencies(rootNode);

    std::string resultingCode = "";
    for (const auto& node : sorted) {
        std::string content = filesystem.readFile(node.path);
        auto parser = createParser(content);
        auto result = parser->parse();
        resultingCode += result.code + '\n';
    }

    return resultingCode;
}
