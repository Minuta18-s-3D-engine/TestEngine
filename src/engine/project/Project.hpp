#ifndef ENGINE_PROJECT_PROJECT_HPP_
#define ENGINE_PROJECT_PROJECT_HPP_

#include <string>
#include <memory>
#include <filesystem>

#include "PathResolver.hpp"
#include "../assets/AssetManager.hpp"
#include "cmakeConfig.h"
#include "VirtualPath.hpp"

class Project {
    std::string name;
    std::string projectVersion;
    std::string engineVersion;

    std::unique_ptr<PathResolver> pathResolver;
    std::unique_ptr<AssetManager> assetManager;

    bool checkEngineVersion();
public:
    Project(
        std::string name, std::string projectVersion, 
        std::string engineVersion, const std::filesystem::path& rootPath,
        const std::filesystem::path& coreRootPath
    );

    Project(const Project&) = delete;
    Project& operator=(const Project&) = delete;

    std::filesystem::path resolve(const std::string& virtualPath) const;

    AssetManager& getAssetManager();
    PathResolver& getPathResolver();

    const std::string& getName() const;
    const std::string& getProjectVersion() const;
    const std::string& getEngineVersion() const;
};

#endif // ENGINE_PROJECT_PROJECT_HPP_
