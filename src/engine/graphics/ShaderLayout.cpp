#include "ShaderLayout.hpp"

bool ShaderLayout::hasProperty(const std::string& name) const {
    return propertyIndex.contains(name);
}

bool ShaderLayout::hasSampler(const std::string& name) const {
    return samplerIndex.contains(name);
}