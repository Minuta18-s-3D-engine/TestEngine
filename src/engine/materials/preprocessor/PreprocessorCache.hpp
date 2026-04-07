#ifndef ENGINE_MATERIALS_PREPROCESSOR_PREPROCESSORCACHE_H_
#define ENGINE_MATERIALS_PREPROCESSOR_PREPROCESSORCACHE_H_

#include <string>

#include "../../project/FilesystemAbstraction.hpp"
#include "PreprocessorParser.hpp"

class PreprocessorCache {
public:
    struct ProcessedShader {
        VirtualPath path;
        std::string source;
        PreprocessorParser::ParseResult parseResult;
        std::vector<VirtualPath> dependencies;
    };
private:
    FilesystemAbstraction& filesystem;
    VirtualPath cacheFolder;
public:
    PreprocessorCache(
        FilesystemAbstraction& _filesystem, 
        VirtualPath _cacheFolder
    );
    ~PreprocessorCache() = default;

    PreprocessorCache(const PreprocessorCache&) = delete;
    PreprocessorCache& operator=(const PreprocessorCache&) = delete;
    PreprocessorCache(PreprocessorCache&&) = default;
    PreprocessorCache& operator=(PreprocessorCache&&) = default;

    void store(const ProcessedShader& processedShader);
    ProcessedShader load(const VirtualPath& sourcePath);

    void clearCache();
    void clearRAMCache();

    VirtualPath getCacheFolder();
};

#endif // ENGINE_MATERIALS_PREPROCESSOR_PREPROCESSORCACHE_H_

