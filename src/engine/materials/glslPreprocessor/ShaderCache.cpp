#include "ShaderCache.hpp"

std::string ShaderCache::getKey(VirtualPath path, ShaderSectionType type) {
    return path.resolve() + shaderSectionTypeMap.enumToString(type);
}
