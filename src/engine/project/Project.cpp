#include "Project.hpp"

Project::Project(
    std::string name, std::string projectVersion, std::string engineVersion,
    const std::filesystem::path& rootPath,
    const std::filesystem::path& coreRootPath
) : name(name), projectVersion(projectVersion), engineVersion(engineVersion) {
    pathResolver = std::make_unique<PathResolver>(rootPath, coreRootPath);
    assetManager = std::make_unique<AssetManager>();

    if (!checkEngineVersion()) {
        std::cerr 
            << "Warning: project was created using wrong engine version."
            << std::endl;
    }

    VirtualPath::setResolverFunc([this](const std::string& p) {
        return this->pathResolver->resolve(p);
    });
}

bool Project::checkEngineVersion() {
    return engineVersion == PROJECT_VERSION;
}

std::filesystem::path Project::resolve(const std::string& virtualPath) const {
    return pathResolver->resolve(virtualPath);
}

AssetManager& Project::getAssetManager() { return *assetManager; };
PathResolver& Project::getPathResolver() { return *pathResolver; }

const std::string& Project::getName() const { return name; }
const std::string& Project::getProjectVersion() const { return projectVersion; }
const std::string& Project::getEngineVersion() const { return engineVersion; }
