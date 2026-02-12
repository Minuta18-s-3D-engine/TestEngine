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

Scene& Project::getActiveScene() {
    if (!hasActiveScene()) throw std::out_of_range("Scene is unset");
    return *(scenes[activeScene]);
}

void Project::setActiveScene(const std::string& sceneName) {
    if (!scenes.contains(sceneName)) 
        throw std::out_of_range("No such scene: " + sceneName);
    isHasActiveScene = true;
    activeScene = sceneName;
}

bool Project::hasActiveScene() {
    return isHasActiveScene;
}

void Project::unloadScene() {
    isHasActiveScene = false;
}

void Project::addScene(std::unique_ptr<Scene> scene) {
    if (!scenes.contains(scene->getName())) 
        throw exc::already_exists("Scene already exists: " + scene->getName());
    scenes[scene->getName()] = std::move(scene);
}

void Project::createEmptyScene(const std::string& sceneName) {
    auto gameObjectManager = std::make_unique<GameObjectManager>();
    auto scene = std::make_unique<Scene>(gameObjectManager, sceneName);
    scenes[sceneName] = std::move(scene);
}

Scene& Project::getScene(const std::string& sceneName) {
    if (!scenes.contains(sceneName)) 
        throw std::out_of_range("No such scene: " + sceneName);
    return *(scenes[sceneName]);
}

bool Project::hasScene(const std::string& sceneName) {
    return (scenes.contains(sceneName));
}

AssetManager& Project::getAssetManager() { return *assetManager; };
PathResolver& Project::getPathResolver() { return *pathResolver; }

const std::string& Project::getName() const { return name; }
const std::string& Project::getProjectVersion() const { return projectVersion; }
const std::string& Project::getEngineVersion() const { return engineVersion; }
