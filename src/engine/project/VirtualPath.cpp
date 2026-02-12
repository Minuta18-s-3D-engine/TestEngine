#include "VirtualPath.hpp"

VirtualPath::ResolverFunc VirtualPath::resolver = 
    [](const std::string& p){ return p; };

VirtualPath::VirtualPath() : path("") {}

VirtualPath::VirtualPath(const std::string& _path) : path(_path) {}

VirtualPath::VirtualPath(const char* _path) : path(_path) {}

VirtualPath::VirtualPath(const std::filesystem::path& _path) 
    : path(_path.string()) {}

std::string VirtualPath::resolve() const {
    if (resolver) {
        return resolver(path);
    }
    return path;
}