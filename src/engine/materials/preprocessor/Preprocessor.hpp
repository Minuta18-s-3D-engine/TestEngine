#ifndef ENGINE_MATERIALS_PREPROCESSOR_PREPROCESSOR_H_
#define ENGINE_MATERIALS_PREPROCESSOR_PREPROCESSOR_H_

#include <string>
#include <unordered_set>
#include <iostream>

#include "PreprocessorParser.hpp"
#include "PreprocessorCache.hpp" 
#include "ShaderDependencyGraph.hpp"   
#include "../../project/FilesystemAbstraction.hpp"

class Preprocessor {
    const std::string IMPORT_DIRECTIVE = "import";
    
    PreprocessorCache cache;
    ShaderDependencyGraph dependencyGraph;

    FilesystemAbstraction& filesystem;
    
    PreprocessorCache::ProcessedShader parseOrLoad(
        const VirtualPath& filePath
    );

    std::shared_ptr<PreprocessorParser> createParser(
        const std::string& fileContents
    );

    void buildDependencyGraph(const VirtualPath& filePath);
public:
    Preprocessor(FilesystemAbstraction& _filesystem);

    std::string preprocess(const VirtualPath& filePath);
};

#endif // ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSOR_H_
