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
    const ProcessedSection& processedSection
) {
    std::string sourceFilename = processedSection.sourcePath.getVirtual() + "_" + \
        processedSection.sectionName;
    return std::to_string(std::hash<std::string>{}(sourceFilename)) + \
        ".cache.glsl";
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

void PreprocessorCache::store(const ProcessedSection& processedSection) {
    std::string filename = getFilename(processedSection);
    nlohmann::json j = serializeProcessedSection(processedSection);
    filesystem.writeFile(cacheFolder.resolve() + filename, j.dump());
}
