#include "MaterialLayout.hpp"

std::string MaterialLayout::getGLSLType(PropertyType t) {
    #define X(name, type, glslType) \
        if (t == PropertyType::name) return glslType ;

        PROPERTY_TYPE_LIST
    #undef X
    return "unknown";
}

size_t MaterialLayout::getStd430Alignment(PropertyType type) {
    switch (type) {
        case PropertyType::Float: case PropertyType::Int: 
        case PropertyType::Uint:  case PropertyType::Bool: 
            return 4;
        case PropertyType::Vec2:  case PropertyType::IVec2: 
        case PropertyType::UVec2: case PropertyType::Mat2: 
            return 8;
        case PropertyType::Vec3:  case PropertyType::IVec3: 
        case PropertyType::UVec3: case PropertyType::UVec4:
        case PropertyType::Vec4:  case PropertyType::IVec4:
        case PropertyType::Mat3: case PropertyType::Mat4: 
            return 16;
        default: 
            return 4;
    }
}

size_t MaterialLayout::getStd430Size(PropertyType type) {
    switch (type) {
        case PropertyType::Vec3:  case PropertyType::UVec3:
        case PropertyType::IVec3:
            return 16;
        case PropertyType::Mat3:
            return 48;
        case PropertyType::Mat4:
            return 64;
        case PropertyType::Vec2:  case PropertyType::IVec2: 
        case PropertyType::UVec2: 
            return 8;
        case PropertyType::Vec4:  case PropertyType::IVec4: 
        case PropertyType::UVec4: case PropertyType::Mat2:  
            return 16;
        default: return 4; 
    }
}

void MaterialLayout::repackData() {
    if (finalized) return;
    
    std::vector<MaterialLayout::SortEntry> sortedProps;
    for (const auto& [name, prop] : properties) {
        sortedProps.push_back({ getStd430Alignment(prop.type), name });
    }

    std::sort(sortedProps.begin(), sortedProps.end(), [](
        const SortEntry& a, const SortEntry& b 
    ) {
        return a.first > b.first;
    });

    size_t currentOffset = 0;
    maxAlignment = 4; // Min. alignment in GLSL is 4 bytes.
    for (const auto& entry : sortedProps) {
        Property& prop = properties.at(entry.second);
        size_t align = entry.first;
        size_t size = getStd430Size(prop.type);

        currentOffset = (currentOffset + align - 1) & ~(align - 1);

        prop.offset = currentOffset;
        prop.size = size;

        currentOffset += size;
        maxAlignment = std::max(maxAlignment, align);
    }

    materialSize = (currentOffset + maxAlignment - 1) & ~(maxAlignment - 1);
    
    propsOrder.reserve(sortedProps.size());
    for (const auto& [size, name] : sortedProps) {
        propsOrder.push_back(name);
    }
}

bool MaterialLayout::hasProperty(const std::string& name) const {
    return properties.contains(name);
}

const MaterialLayout::Property& MaterialLayout::getPropertyInfo(
    const std::string& name
) const {
    return properties.at(name);
}

size_t MaterialLayout::getLayoutSize() const {
    if (!finalized) return 0;
    
    return materialSize;
}

void MaterialLayout::finalize() {
    repackData();

    finalized = true;
};

bool MaterialLayout::isFinalized() const {
    return finalized;
}

std::vector<std::string> MaterialLayout::getPropertyOrder() const {
    if (!finalized) {
        throw std::invalid_argument(
            "Unable to get property order before finalization"
        );
    }

    return propsOrder;
}