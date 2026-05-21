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
        case PropertyType::Int64: case PropertyType::Uint64: 
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
        case PropertyType::UVec2: case PropertyType::Int64:
        case PropertyType::Uint64: 
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
        sortedProps.push_back({ 
            getStd430Alignment(prop.type),
            getStd430Size(prop.type),
            name 
        });
    }

    std::sort(sortedProps.begin(), sortedProps.end(), [](
        const MaterialLayout::SortEntry& a, 
        const MaterialLayout::SortEntry& b 
    ) {
        if (a.alignment != b.alignment) return a.alignment > b.alignment;
        if (a.size != b.size) return a.size > b.size;
        return a.name < b.name;
    });

    size_t currentOffset = 0;
    maxAlignment = 4; // Min. alignment in GLSL is 4 bytes.
    for (const auto& entry : sortedProps) {
        Property& prop = properties.at(entry.name);

        currentOffset = (currentOffset + entry.alignment - 1) & 
            ~(entry.alignment - 1);

        prop.offset = currentOffset;
        prop.size = entry.size;

        currentOffset += entry.size;
        maxAlignment = std::max(maxAlignment, entry.alignment);
    }

    materialSize = (currentOffset + maxAlignment - 1) & ~(maxAlignment - 1);
    
    propsOrder.reserve(sortedProps.size());
    for (const auto& prop: sortedProps) {
        propsOrder.push_back(prop.name);
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