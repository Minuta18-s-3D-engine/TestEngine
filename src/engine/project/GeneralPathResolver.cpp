#include "GeneralPathResolver.hpp"

void GeneralPathResolver::addPrefix(
    std::string name, std::filesystem::path path
) {
    prefixes[name] = path;
}

std::string GeneralPathResolver::resolveFrom(
    const std::string& virtualPath,
    const std::string& folder 
) const {
    if (virtualPath.empty()) return "";

    for (auto& [ prefix, path ] : prefixes) {
        if (virtualPath.compare(0, prefix.size(), prefix) == 0) {
            return path / virtualPath.substr(prefix.size());
        }
    }

    std::filesystem::path p(virtualPath);
    if (p.is_absolute()) return p;

    return std::filesystem::path(folder) / virtualPath;
}

std::string GeneralPathResolver::resolve(
    const std::string& virtualPath
) const {
    return resolveFrom(virtualPath, "./");
}
