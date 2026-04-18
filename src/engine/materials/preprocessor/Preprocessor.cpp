#include "Preprocessor.hpp"

Preprocessor::Preprocessor(
    FilesystemAbstraction& _filesystem
) : filesystem(_filesystem), 
    cache(_filesystem, "fs://.cache/materialCode") {}

PreprocessorCache::ProcessedShader Preprocessor::parseOrLoad(
    const VirtualPath& filePath,
    ShaderDiagnostic& diagnostic
) {
    {
        auto processedShader = cache.load(filePath);
        if (processedShader.has_value()) {
            return processedShader.value();
        }
    }

    std::string contents = filesystem.readFile(filePath);
    auto parser = createParser(contents, filePath, diagnostic);
    auto result = parser->parse();

    std::vector<VirtualPath> dependencies;
    for (auto& directive : result.directives) {
        if (!directive.nameMatch({ IMPORT_DIRECTIVE })) continue;

        const size_t argIndex = 0;
        if (directive.args.size() < argIndex + 1) continue;
        if (
            directive.args.at(argIndex).type != 
            PreprocessorParser::ArgType::String
        ) {
            continue;
        }
        
        dependencies.push_back(std::string(directive.args.at(argIndex).value));
    }

    PreprocessorCache::ProcessedShader cacheEntry(
        filePath,
        result.code,
        dependencies,
        filesystem.getLastEditedTime(filePath)
    );

    cache.store(cacheEntry);

    return cacheEntry;
}

void Preprocessor::buildDependencyGraph(
    const VirtualPath& filePath,
    ShaderDiagnostic& diagnostic
) {
    ShaderDependencyGraph::NodeId nodeId(filePath);

    if (dependencyGraph.nodeExists(nodeId)) return;

    PreprocessorCache::ProcessedShader parsedShader = parseOrLoad(
        filePath, diagnostic
    );
    dependencyGraph.addNode(nodeId);

    for (auto& path : parsedShader.dependencies) {
        buildDependencyGraph(path, diagnostic);
        ShaderDependencyGraph::NodeId depNodeId(path);
        dependencyGraph.addDependency(nodeId, depNodeId);
    }
}

std::shared_ptr<PreprocessorParser> Preprocessor::createParser(
    const std::string& fileContents,
    const VirtualPath& path,
    ShaderDiagnostic& diagnostic
) {
    auto parser = std::make_shared<PreprocessorParser>(
        fileContents, path, diagnostic);

    using ArgType = PreprocessorParser::ArgType;
    parser->addDirectiveValidator(
        IMPORT_DIRECTIVE, { ArgType::String }
    );

    return parser;
}

std::pair<std::string, ShaderDiagnostic> Preprocessor::preprocess(
    const VirtualPath& filePath
) {
    ShaderDiagnostic diagnostic;

    try {
        ShaderDependencyGraph::NodeId rootNode(filePath);
        buildDependencyGraph(filePath, diagnostic);

        auto sorted = dependencyGraph.getSortedDependencies(rootNode);

        std::stringstream resultingCode;
        for (const auto& node : sorted) {
            std::string content = filesystem.readFile(node.path);
            auto parser = createParser(content, filePath, diagnostic);
            auto result = parser->parse();
            resultingCode << result.code << '\n';
        }

        return { resultingCode.str(), diagnostic };
    } catch (std::invalid_argument& e) {
        diagnostic.report(
            ShaderDiagnostic::IssueType::Error,
            e.what()
        );
        throw ShaderDiagnostic::preprocessor_error(e.what(), diagnostic);
    } catch (loop_detected& e) {
        diagnostic.report(
            ShaderDiagnostic::IssueType::Error,
            e.what()
        );
        throw ShaderDiagnostic::preprocessor_error(e.what(), diagnostic);
    }
}
