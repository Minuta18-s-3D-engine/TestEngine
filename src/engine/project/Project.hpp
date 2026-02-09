#ifndef ENGINE_PROJECT_PROJECT_HPP_
#define ENGINE_PROJECT_PROJECT_HPP_

#include <string>
#include <memory>

#include "../assets/AssetManager.hpp"

class Project {
    std::string name;
    std::string projectVersion;

    std::unique_ptr<AssetManager> assetManager;
public:
    Project(std::string name, std::string projectVersion);

    Project(const Project&) = delete;
    Project& operator=(const Project&) = delete;

    std::string getName() const;
    std::string getProjectVersion() const;

    AssetManager& getAssetManager();
};

#endif // ENGINE_PROJECT_PROJECT_HPP_
