#ifndef ENGINE_PROJECT_PROJECT_HPP_
#define ENGINE_PROJECT_PROJECT_HPP_

#include <string>
#include <memory>
#include <filesystem>

#include "PathResolver.hpp"
#include "../assets/AssetManager.hpp"

class Project {
    std::string name;
    std::string projectVersion;

    std::unique_ptr<PathResolver> pathResolver;
    std::unique_ptr<AssetManager> assetManager;
public:
    Project(
        std::string name, std::string projectVersion, 
        const std::filesystem::path& rootPath
    );

    Project(const Project&) = delete;
    Project& operator=(const Project&) = delete;

    std::filesystem::path resolve(const std::string& virtualPath) const;

    AssetManager& getAssetManager();
    PathResolver& getPathResolver();

    std::string getName() const;
    std::string getProjectVersion() const;
};

#endif // ENGINE_PROJECT_PROJECT_HPP_
