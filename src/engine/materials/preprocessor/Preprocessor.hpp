#ifndef ENGINE_MATERIALS_PREPROCESSOR_PREPROCESSOR_H_
#define ENGINE_MATERIALS_PREPROCESSOR_PREPROCESSOR_H_

#include <string>

#include "PreprocessorParser.hpp"
#include "PreprocessorCache.hpp" 
#include "ShaderDependencyGraph.hpp"   

class Preprocessor {
    PreprocessorCache cache;
    ShaderDependencyGraph dependencyGraph;
    PreprocessorParser parser;
public:
    void preprocess(const VirtualPath& filePath);
};

#endif // ENGINE_MATERIALS_GLSLPREPROCESSOR_PREPROCESSOR_H_
