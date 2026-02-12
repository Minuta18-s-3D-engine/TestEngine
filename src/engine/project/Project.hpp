#ifndef ENGINE_PROJECT_PROJECT_HPP_
#define ENGINE_PROJECT_PROJECT_HPP_

#include <string>
#include <memory>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <stdexcept>

#include "PathResolver.hpp"
#include "../assets/AssetManager.hpp"
#include "cmakeConfig.h"
#include "VirtualPath.hpp"
#include "../scene/Scene.hpp"
#include "../utils/exc/GeneralExceptions.hpp"

class Project {
    std::string name;
    std::string projectVersion;
    std::string engineVersion;

    std::unique_ptr<PathResolver> pathResolver;
    std::unique_ptr<AssetManager> assetManager;

    std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;
    std::string activeScene;
    bool isHasActiveScene = false;

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

    Scene& getActiveScene();
    void setActiveScene(const std::string& sceneName);
    bool hasActiveScene();
    void unloadScene();

    void addScene(std::unique_ptr<Scene> scene);
    void createEmptyScene(const std::string& sceneName);
    Scene& getScene(const std::string& sceneName);
    bool hasScene(const std::string& sceneName);

    AssetManager& getAssetManager();
    PathResolver& getPathResolver();

    const std::string& getName() const;
    const std::string& getProjectVersion() const;
    const std::string& getEngineVersion() const;
};

#endif // ENGINE_PROJECT_PROJECT_HPP_
