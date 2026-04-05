#include "VirtualFilesystem.hpp"

VirtualFilesystem::VirtualFilesystem(
    std::unique_ptr<PathResolver> _pathResolver
) : pathResolver(std::move(_pathResolver)) {
    VirtualPath::setResolverFunc([this](const std::string& p) {
        return this->getResolver().resolve(p);
    });
}

VirtualFilesystem::~VirtualFilesystem() {
    VirtualPath::setResolverFunc(nullptr);
}

bool VirtualFilesystem::exists(const VirtualPath& path) {
    return std::filesystem::exists(path.resolve());
}

std::string VirtualFilesystem::readFile(const VirtualPath& path) {
    std::filesystem::path physicalPath = path.resolve();
    if (!exists(path)) {
        throw exc::file_not_found("File not found: " + physicalPath.string());
    }
    if (!isFile(physicalPath)) {
        throw exc::not_a_file("Not a file: " + physicalPath.string());
    }

    std::ifstream file(physicalPath, std::ios::in | std::ios::binary);
    auto fileSize = std::filesystem::file_size(physicalPath);
    std::string buffer(fileSize, '\0');
    file.read(buffer.data(), fileSize);
    file.close();
    
    return buffer;
}

void VirtualFilesystem::writeFile(
    const VirtualPath& path, 
    const std::string& data, 
    bool overwrite
) {
    std::filesystem::path physicalPath = path.resolve();
        
    if (!overwrite && exists(physicalPath)) {
        throw exc::already_exists(
            "File already exists: " + physicalPath.string()
        );
    }

    if (physicalPath.has_parent_path()) {
        std::filesystem::create_directories(physicalPath.parent_path());
    }

    std::ofstream file(physicalPath, std::ios::out | std::ios::binary);
    file.write(data.data(), data.size());
    file.close();
}

bool VirtualFilesystem::isFile(const VirtualPath& path) {
    return std::filesystem::is_regular_file(path.resolve());
}

bool VirtualFilesystem::fileExists(const VirtualPath& path) {
    if (!exists(path)) return false;
    if (!isFile(path)) return false;
    return true;
}

std::filesystem::file_time_type VirtualFilesystem::getLastEditedTime(
    const VirtualPath& path
) {
    return std::filesystem::last_write_time(path.resolve());
}

bool VirtualFilesystem::dirExists(const VirtualPath& path) {
    return std::filesystem::is_directory(path.resolve());
}

void VirtualFilesystem::createDir(const VirtualPath& path) {
    std::filesystem::create_directories(path.resolve());
}

void VirtualFilesystem::removeDir(const VirtualPath& path, bool recursively) {
    if (recursively) {
        std::filesystem::remove_all(path.resolve());
    } else {
        std::filesystem::remove(path.resolve());
    }
}

PathResolver& VirtualFilesystem::getResolver() {
    return *pathResolver;
}
