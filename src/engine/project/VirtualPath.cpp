#include "VirtualPath.hpp"

std::shared_ptr<PathResolver> VirtualPath::resolver = nullptr;

VirtualPath::VirtualPath() : path("") {}

VirtualPath::VirtualPath(const std::string& _path) : path(_path) {}

VirtualPath::VirtualPath(const char* _path) : path(_path) {}

VirtualPath::VirtualPath(const std::filesystem::path& _path) 
    : path(_path.string()) {}

std::string VirtualPath::resolve() const {
    if (resolver) {
        return resolver->resolve(path);
    }
    return path;
}

std::string VirtualPath::resolveFrom(const VirtualPath& folder) const {
    if (resolver) {
        return resolver->resolveFrom(path, folder.resolve());
    }
    return path;
}
