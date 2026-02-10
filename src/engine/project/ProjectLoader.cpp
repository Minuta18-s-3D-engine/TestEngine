#include "ProjectLoader.hpp"

nlohmann::json ProjectLoader::readJsonFile(const std::ifstream& fileStream) {
    try {
        nlohmann::json result = nlohmann::json::parse(fileStream);
        return result;
    } catch (const nlohmann::json::parse_error& e) {
        throw exc::parse_error(e.what());
    }
}

std::unique_ptr<Project> ProjectLoader::loadProject(
    const std::filesystem::path& folderPath
) {
    std::filesystem::path configPath = folderPath / projectConfigFilename;
    if (!std::filesystem::exists(configPath)) {
        throw exc::file_not_found(std::format(
            "Project config file {} not found.",
            configPath
        ));
    }

    std::ifstream configFile(configPath);
    nlohmann::json config = readJsonFile(configFile);

    if (!config.contains("name")) {
        throw exc::validation_error("Mandatory field \"name\" not found.");
    }

    if (!config.contains("projectVersion")) {
        throw exc::validation_error(
            "Mandatory field \"projectVersion\" not found.");
    }

    std::unique_ptr<Project> project = std::make_unique<Project>(
        config["name"],
        config["projectVersion"]
    );

    return project;
}
