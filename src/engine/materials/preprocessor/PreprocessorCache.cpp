#include "PreprocessorCache.hpp"

PreprocessorCache::PreprocessorCache(
    FilesystemAbstraction& _filesystem, 
    VirtualPath _cacheFolder
) : filesystem(_filesystem), cacheFolder(_cacheFolder) {
    createCacheFolder();
}

void PreprocessorCache::createCacheFolder() {
    if (!filesystem.folderExists(cacheFolder)) {
        filesystem.createFolder(cacheFolder);
    }
}

std::string PreprocessorCache::getFilename(
    const VirtualPath& sourcePath, std::string section
) {
    std::string sourceFilename = sourcePath.getVirtual() + "_" + \
        section;
    size_t filenameHash = std::hash<std::string>{}(sourceFilename);
    std::stringstream hashedFilename;
    hashedFilename << std::hex << filenameHash;
    return hashedFilename.str() + ".cache.glsl";
}
    
nlohmann::json PreprocessorCache::serializeProcessedSection(
    const ProcessedSection& processedSection
) {
    nlohmann::json j;
    j["meta"]["sourcePath"] = processedSection.sourcePath.getVirtual();
    j["meta"]["sectionName"] = processedSection.sectionName;
    j["glslCode"] = processedSection.preprocessedCode;
    j["dependencies"] = nlohmann::json::array();
    for (const auto& dep : processedSection.dependencies) {
        j["dependencies"].push_back(dep.getVirtual());
    }
    return j;
}

PreprocessorCache::ProcessedSection 
PreprocessorCache::deserializeProcessedSection(
    const std::string& content
) {
    nlohmann::json j = nlohmann::json::parse(content);
    ProcessedSection section;
    section.sourcePath = VirtualPath(
        j.at("meta").at("sourcePath").get<std::string>()
    );
    section.sectionName = j.at("meta").at("sectionName").get<std::string>();
    section.preprocessedCode = j.at("glslCode").get<std::string>();
    section.dependencies.reserve(j.at("dependencies").size());
    for (auto& elm : j.at("dependencies")) {
        section.dependencies.push_back(elm.get<std::string>());
    }

    return section;
}

std::optional<PreprocessorCache::ProcessedSection> PreprocessorCache::load(
    const VirtualPath& sourcePath, std::string section
) {
    std::string exceptedFilename = getFilename(sourcePath, section);
    if (!filesystem.fileExists(exceptedFilename)) {
        return std::nullopt;
    }

    std::string contents = filesystem.readFile(exceptedFilename);
    return deserializeProcessedSection(contents);
}

void PreprocessorCache::store(const ProcessedSection& processedSection) {
    std::string filename = getFilename(
        processedSection.sourcePath, processedSection.sectionName);
    nlohmann::json j = serializeProcessedSection(processedSection);
    filesystem.writeFile(cacheFolder.resolve() + "/" + filename, j.dump());
}

bool PreprocessorCache::exists(const ProcessedSection& processedSection) {
    std::string filename = getFilename(
        processedSection.sourcePath, processedSection.sectionName);
    return filesystem.fileExists(filename);
}

