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

PreprocessorCache::ProcessedSection PreprocessorCache::load(
    const VirtualPath& sourcePath, std::string section
) {
    // WIP
    std::string exceptedFilename = getFilename();
    if (!filesystem.fileExists())

    std::string contents = file.
}

void PreprocessorCache::store(const ProcessedSection& processedSection) {
    std::string filename = getFilename(processedSection);
    nlohmann::json j = serializeProcessedSection(processedSection);
    filesystem.writeFile(cacheFolder.resolve() + "/" + filename, j.dump());
}

bool PreprocessorCache::exists(const ProcessedSection& processedSection) {
    std::string filename = getFilename(processedSection);
    return filesystem.fileExists(filename);
}

