#ifndef ENGINE_MATERIALS_PREPROCESSOR_PREPROCESSOR_H_
#define ENGINE_MATERIALS_PREPROCESSOR_PREPROCESSOR_H_

#include <string>
#include <unordered_set>
#include <iostream>
#include <sstream>

#include "PreprocessorParser.hpp"
#include "PreprocessorCache.hpp" 
#include "ShaderDependencyGraph.hpp"   
#include "../../project/FilesystemAbstraction.hpp"
#include "ShaderDiagnostic.hpp"

class Preprocessor {
    const std::string IMPORT_DIRECTIVE = "import";
    
    PreprocessorCache cache;
    ShaderDependencyGraph dependencyGraph;

    FilesystemAbstraction& filesystem;
    
    PreprocessorCache::ProcessedShader parseOrLoad(
        const VirtualPath& filePath,
        ShaderDiagnostic& diagnostic
    );

    std::shared_ptr<PreprocessorParser> createParser(
        const std::string& fileContents,
        const VirtualPath& path,
        ShaderDiagnostic& diagnostic
    );

    void buildDependencyGraph(
        const VirtualPath& filePath,
        ShaderDiagnostic& diagnostic
    );
public:
    Preprocessor(FilesystemAbstraction& _filesystem);

    std::pair<std::string, ShaderDiagnostic> preprocess(
        const VirtualPath& filePath
    );
};

#endif // ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSOR_H_
