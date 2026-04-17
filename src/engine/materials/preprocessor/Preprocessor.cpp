#include "Preprocessor.hpp"

Preprocessor::Preprocessor(
    FilesystemAbstraction& _filesystem
) : filesystem(_filesystem), 
    cache(_filesystem, "fs://.cache/materialCode") {}

void Preprocessor::buildDependencyGraph(
    const VirtualPath& filePath
) {
    ShaderDependencyGraph::NodeId nodeId(filePath);

    if (dependencyGraph.nodeExists(nodeId)) return;

    std::string contents = filesystem.readFile(filePath);
    auto parser = createParser(contents);
    auto result = parser->parse();

    for (auto& directive : result.directives) {
        if (directive.nameMatch({"import"})) {
            std::string importPathStr = std::string(directive.args[0].value);
            VirtualPath importPath(importPathStr);
            buildDependencyGraph(importPath);
            ShaderDependencyGraph::NodeId depNodeId(importPath);
            dependencyGraph.addDependency(nodeId, depNodeId);
        }
    }
}

std::shared_ptr<PreprocessorParser> Preprocessor::createParser(
    const std::string& fileContents
) {
    auto parser = std::make_shared<PreprocessorParser>(fileContents);

    using ArgType = PreprocessorParser::ArgType;
    parser->addDirectiveValidator(
        "import", { ArgType::String }
    );

    return parser;
}

void Preprocessor::preprocess(const VirtualPath& filePath) {
    std::string fileContents = filesystem.readFile(filePath);
    
    auto parser = createParser(fileContents);
    auto result = parser->parse();
}
