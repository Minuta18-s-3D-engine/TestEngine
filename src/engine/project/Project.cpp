#include "Project.hpp"

Project::Project(
    std::string name, std::string projectVersion,
    const std::filesystem::path& rootPath
) : name(name), projectVersion(projectVersion) {
    pathResolver = std::make_unique<PathResolver>(rootPath);
    assetManager = std::make_unique<AssetManager>();
}

std::filesystem::path Project::resolve(const std::string& virtualPath) const {
    return pathResolver->resolve(virtualPath);
}

AssetManager& Project::getAssetManager() { return *assetManager; };
PathResolver& Project::getPathResolver() { return *pathResolver; }

std::string Project::getName() const { return name; }
std::string Project::getProjectVersion() const { return projectVersion; }
