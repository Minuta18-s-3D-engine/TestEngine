#ifndef ENGINE_PROJECT_PROJECTLOADER_HPP_
#define ENGINE_PROJECT_PROJECTLOADER_HPP_

#include <json.hpp>

#include <filesystem>
#include <string>
#include <stdexcept>
#include <format>
#include <fstream>

#include "Project.hpp"
#include "../utils/exc/ParseExceptions.hpp"

class ProjectLoader {
    std::string projectConfigFilename = "project.json";

    nlohmann::json readJsonFile(const std::ifstream& fileStream);
public:
    std::unique_ptr<Project> loadProject(
        const std::filesystem::path& folderPath
    );
};

#endif // ENGINE_PROJECT_PROJECTLOADER_HPP_
