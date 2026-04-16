#ifndef ENGINE_MATERIALS_PREPROCESSOR_PREPROCESSOR_H_
#define ENGINE_MATERIALS_PREPROCESSOR_PREPROCESSOR_H_

#include <string>

#include "PreprocessorParser.hpp"
#include "PreprocessorCache.hpp" 
#include "ShaderDependencyGraph.hpp"   
#include "../../project/FilesystemAbstraction.hpp"

class Preprocessor {
    PreprocessorCache cache;
    ShaderDependencyGraph dependencyGraph;

    FilesystemAbstraction& filesystem;

    std::shared_ptr<PreprocessorParser> createParser(
        const std::string& fileContents
    );
public:
    Preprocessor(FilesystemAbstraction& _filesystem);

    void preprocess(const VirtualPath& filePath);
};

#endif // ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSOR_H_
