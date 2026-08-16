#include "ShaderLayout.hpp"

#include <algorithm>

void ShaderLayout::repackData() {
    if (finalized) return;

    propertyOrder.clear();
    propertyOrder.reserve(properties.size());
    for (const auto& [name, _] : properties) {
        propertyOrder.push_back(name);
    }
    std::sort(propertyOrder.begin(), propertyOrder.end());

    size_t currentOffset = 0;
    maxAlignment = 4;

    for (const std::string& name : propertyOrder) {
        PropertyInfo& prop = properties.at(name);

        prop.offset = currentOffset;
        prop.size = getSize(prop.type);

        currentOffset += prop.size;
    }

    layoutSize = currentOffset;
}

bool ShaderLayout::hasProperty(const std::string &name) const {
    return properties.contains(name);
}

const ShaderLayout::PropertyInfo& ShaderLayout::getPropertyInfo(
    const std::string &name
) const {
    return properties.at(name);
}

size_t ShaderLayout::getLayoutSize() const {
    if (!finalized) return 0;

    return layoutSize;
}

void ShaderLayout::finalize() {
    repackData();

    finalized = true;
}

bool ShaderLayout::isFinalized() const {
    return finalized;
}

std::vector<std::string> ShaderLayout::getProperties() const {
    if (!finalized) {
        throw std::invalid_argument(
            "Unable to get property order before finalization"
        );
    }
    return propsOrder;
}
