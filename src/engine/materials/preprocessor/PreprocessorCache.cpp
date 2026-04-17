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
    const VirtualPath& sourcePath
) {
    std::string sourceFilename = sourcePath.getVirtual();
    size_t filenameHash = std::hash<std::string>{}(sourceFilename);
    std::stringstream hashedFilename;
    hashedFilename << std::hex << filenameHash;
    return hashedFilename.str() + ".cache.json";
}
    
nlohmann::json PreprocessorCache::serializeProcessedShader(
    const ProcessedShader& processedShader
) {
    nlohmann::json j;
    j["meta"]["sourcePath"] = processedShader.sourcePath.getVirtual();
    j["glslCode"] = processedShader.preprocessedCode;
    j["dependencies"] = nlohmann::json::array();
    for (const auto& dep : processedShader.dependencies) {
        j["dependencies"].push_back(dep.getVirtual());
    }
    return j;
}

PreprocessorCache::ProcessedShader 
PreprocessorCache::deserializeProcessedShader(
    const std::string& content
) {
    nlohmann::json j = nlohmann::json::parse(content);
    ProcessedShader section;
    section.sourcePath = VirtualPath(
        j.at("meta").at("sourcePath").get<std::string>()
    );
    section.preprocessedCode = j.at("glslCode").get<std::string>();
    section.dependencies.reserve(j.at("dependencies").size());
    for (auto& elm : j.at("dependencies")) {
        section.dependencies.push_back(elm.get<std::string>());
    }

    return section;
}

std::optional<PreprocessorCache::ProcessedShader> PreprocessorCache::load(
    const VirtualPath& sourcePath
) {
    std::string exceptedFilename = getFilename(sourcePath);
    VirtualPath exceptedPath(cacheFolder.getVirtual() + "/" + exceptedFilename);
    if (!filesystem.fileExists(exceptedPath)) {
        return std::nullopt;
    }

    std::string contents = filesystem.readFile(exceptedPath);
    return deserializeProcessedShader(contents);
}

void PreprocessorCache::store(const ProcessedShader& processedShader) {
    std::string filename = getFilename(processedShader.sourcePath);
    nlohmann::json j = serializeProcessedShader(processedShader);
    filesystem.writeFile(cacheFolder.resolve() + "/" + filename, j.dump());
}

bool PreprocessorCache::exists(const VirtualPath& sourcePath) {
    std::string filename = getFilename(sourcePath);
    VirtualPath exceptedPath(cacheFolder.getVirtual() + "/" + filename);
    return filesystem.fileExists(exceptedPath);
}

VirtualPath PreprocessorCache::getCacheFolder() {
    return cacheFolder;
}
