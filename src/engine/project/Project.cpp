#include "Project.hpp"

Project::Project(
    std::string name, std::string projectVersion
) : name(name), projectVersion(projectVersion) {

}

std::string Project::getName() const { return name; }
std::string Project::getProjectVersion() const { return projectVersion; }
AssetManager& Project::getAssetManager() { return *assetManager; }