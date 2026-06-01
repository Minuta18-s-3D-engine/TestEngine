#ifndef ENGINE_MATERIALS_PREPROCESSOR_PREPROCESSORCACHE_H_
#define ENGINE_MATERIALS_PREPROCESSOR_PREPROCESSORCACHE_H_

#include <nlohmann/json.hpp>

#include <string>
#include <sstream>
#include <optional>
#include <iostream>

#include "../../project/FilesystemAbstraction.hpp"
#include "PreprocessorParser.hpp"

class PreprocessorCache {
public:
    struct ProcessedShader {
        VirtualPath sourcePath;
        std::string preprocessedCode;
        std::vector<VirtualPath> dependencies;
        FilesystemAbstraction::Timestamp sourceEditTime;
    };
private:
    FilesystemAbstraction& filesystem;
    VirtualPath cacheFolder;

    void createCacheFolder();

    std::string getFilename(
        const VirtualPath& sourcePath
    );

    nlohmann::json serializeProcessedShader(
        const ProcessedShader& processedShader
    );
    ProcessedShader deserializeProcessedShader(const std::string& content);
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
    std::optional<ProcessedShader> load(
        const VirtualPath& sourcePath
    );
    bool isUpToDate(const ProcessedShader& processedShader);

    VirtualPath getCacheFolder();
};

#endif // ENGINE_MATERIALS_PREPROCESSOR_PREPROCESSORCACHE_H_

