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
    propsOrder.clear();
    propsOrder.reserve(properties.size());
    for (const auto& [name, _] : properties) {
        propsOrder.push_back(name);
    }
    std::sort(propsOrder.begin(), propsOrder.end());

    size_t currentOffset = 0;
    maxAlignment = 4;

    for (const std::string& name : propsOrder) {
        Property& prop = properties.at(name);

        prop.offset = currentOffset;
        prop.size = getSize(prop.type);

        currentOffset += prop.size;
    }

    materialSize = currentOffset;
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