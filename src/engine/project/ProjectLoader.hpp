#ifndef ENGINE_PROJECT_PROJECTLOADER_HPP_
#define ENGINE_PROJECT_PROJECTLOADER_HPP_

#include <json.hpp>

#include <filesystem>

#include "Project.hpp"

class ProjectLoader {
public:
    std::unique_ptr<Project> loadProject(const std::filesystem::path& path);
};

#endif // ENGINE_PROJECT_PROJECTLOADER_HPP_
