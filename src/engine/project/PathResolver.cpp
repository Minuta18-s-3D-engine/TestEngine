#include "PathResolver.hpp"

PathResolver::PathResolver(
    const std::filesystem::path& _root,
    const std::filesystem::path& _coreRoot
) : root(_root), coreRoot(_coreRoot) {}

std::filesystem::path PathResolver::resolve(
    const std::string& virtualPath
) const {
    if (virtualPath.empty()) return "";

    if (virtualPath.compare(0, prefix.size(), prefix) == 0) {
        return root / virtualPath.substr(prefix.size());
    }

    if (virtualPath.compare(0, corePrefix.size(), corePrefix) == 0) {
        return coreRoot / virtualPath.substr(corePrefix.size());
    }

    std::filesystem::path p(virtualPath);
    if (p.is_absolute()) return p;

    return root / virtualPath;
}

