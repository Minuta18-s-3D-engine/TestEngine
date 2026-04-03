#include "ShaderSourceCache.hpp"

ShaderSourceCache::ShaderSourceCache(
    FilesystemAbstraction& _abstraction, 
    VirtualPath _cachePath
) : filesystem(_abstraction), cachePath(_cachePath) {}

VirtualPath ShaderSourceCache::constructPath(
    std::string name, 
    std::string section
) {
    auto sanitize = [](std::string s) {
        for (auto& c : s) {
            if (c == '/' || c == '\\' || c == ':') {
                c = '_';
            }
        }
        return s;
    };

    std::string filename = sanitize(name) + "_" + sanitize(section);
    return VirtualPath(cachePath.getVirtual() + "/" + filename);
}

