#ifndef ENGINE_MATERIALS_PREPROCESSOR_SHADERSOURCECACHE_H_
#define ENGINE_MATERIALS_PREPROCESSOR_SHADERSOURCECACHE_H_

#include <unordered_map>
#include <memory>
#include <string>
#include <filesystem>

#include "../../project/VirtualPath.hpp"
#include "PreprocessorParser.hpp"
#include "../../project/FilesystemAbstraction.hpp"

class CachedShaderSource {
public:
    struct Metadata {
        VirtualPath sourceFilePath;
        std::filesystem::file_time_type sourceFileLastEdit;
    };

    std::string processedSource;
    std::string name;
    std::string section;
    Metadata meta;
};

class ShaderSourceCache {
    FilesystemAbstraction& filesystem;

    VirtualPath cachePath;
public:
    ShaderSourceCache(
        FilesystemAbstraction& _abstraction, 
        VirtualPath cachePath
    );

    bool exists(std::string name, std::string section);
    CachedShaderSource get(std::string name, std::string section);
    void set(CachedShaderSource shader);

    VirtualPath constructPath(std::string, std::string section);

    void clear();
};

#endif // ENGINE_MATERIALS_PREPROCESSOR_SHADERSOURCECACHE_H_
