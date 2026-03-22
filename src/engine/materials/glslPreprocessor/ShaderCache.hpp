#ifndef ENGINE_MATERIALS_GLSLPREPROCESSOR_SHADERCACHE_H_
#define ENGINE_MATERIALS_GLSLPREPROCESSOR_SHADERCACHE_H_

#include <unordered_map>

#include "ShaderSectionType.hpp"
#include "../../project/VirtualPath.hpp"
#include "PreprocessorParser.hpp"

class ShaderCache {
public:
    struct SectionCache {
        PreprocessorParser::SectionBlock parseResult;
    };
private:
    std::unordered_map<std::string, SectionCache> shaders;

    std::string getKey(VirtualPath path, ShaderSectionType type);
public:
    ShaderCache() = default;
    ~ShaderCache() = default;

    bool hasShader(VirtualPath path, ShaderSectionType type);
    SectionCache getShader(VirtualPath path, ShaderSectionType type);
    void setShader(
        VirtualPath path, 
        ShaderSectionType type, 
        SectionCache cache
    );

    void clearCache();
};

#endif // ENGINE_MATERIALS_GLSLPREPROCESSOR_SHADERCACHE_H_
