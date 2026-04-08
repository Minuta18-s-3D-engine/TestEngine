#ifndef ENGINE_MATERIALS_PREPROCESSOR_PREPROCESSORCACHE_H_
#define ENGINE_MATERIALS_PREPROCESSOR_PREPROCESSORCACHE_H_

#include <nlohmann/json.hpp>

#include <string>
#include <sstream>

#include "../../project/FilesystemAbstraction.hpp"
#include "PreprocessorParser.hpp"

class PreprocessorCache {
public:
    struct ProcessedSection {
        VirtualPath sourcePath;
        std::string sectionName;
        std::string preprocessedCode;
        std::vector<VirtualPath> dependencies;
    };
private:
    FilesystemAbstraction& filesystem;
    VirtualPath cacheFolder;

    void createCacheFolder();
    std::string getFilename(const ProcessedSection& processedSection);
    nlohmann::json serializeProcessedSection(
        const ProcessedSection& processedSection
    );
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

    void store(const ProcessedSection& processedSection);
    ProcessedSection load(const VirtualPath& sourcePath, std::string section);
    bool exists(const ProcessedSection& processedSection);

    void clearCache();

    VirtualPath getCacheFolder();
};

#endif // ENGINE_MATERIALS_PREPROCESSOR_PREPROCESSORCACHE_H_

