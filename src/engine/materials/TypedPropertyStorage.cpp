#include "TypedPropertyStorage.hpp"

TypedPropertyStorage::TypedPropertyStorage() {}

TypedPropertyStorage::~TypedPropertyStorage() {
    freeData();
}

TypedPropertyStorage::TypedPropertyStorage(const TypedPropertyStorage& other) {
    properties = other.properties;
    dataSize = other.dataSize;
    if (other.data) {
        reallocData(dataSize);
        std::memcpy(data, other.data, dataSize);
    }
}

TypedPropertyStorage::TypedPropertyStorage(
    TypedPropertyStorage&& other
) noexcept : properties(std::move(other.properties)), 
    data(other.data), dataSize(other.dataSize) {
    other.data = nullptr;
    other.dataSize = 0;
}

TypedPropertyStorage& TypedPropertyStorage::operator=(
    const TypedPropertyStorage& other
) {
    if (this == &other) return *this;
    if (data) freeData();

    properties = other.properties;
    dataSize = other.dataSize;
    if (other.data) {
        reallocData(dataSize);
        std::memcpy(data, other.data, dataSize);
    }
}

TypedPropertyStorage& TypedPropertyStorage::operator=(
    TypedPropertyStorage&& other
) noexcept {
    if (this == &other) return *this;
    if (data) freeData();

    properties = std::move(other.properties);
    data = other.data;
    dataSize = other.dataSize;

    other.data = nullptr;
    other.dataSize = 0;
    return *this;
}

void TypedPropertyStorage::reallocData(size_t n) {
    if (n == dataSize) return;

    uint8_t* newData = new uint8_t[n];
    if (data) {
        std::memcpy(newData, data, std::min(n, dataSize));
        freeData();
    }
    dataSize = n;
    data = newData;
}

void TypedPropertyStorage::freeData() {
    if (dataSize > 0) {
        delete[] data;
    }
}

bool TypedPropertyStorage::hasProperty(const std::string& name) const {
    return properties.contains(name);
}

bool TypedPropertyStorage::isPropertySet(const std::string& name) const {
    if (!hasProperty(name)) {
        throw std::invalid_argument("No such property: " + name);
    }
    return properties.at(name).isDefaultDataSet;
}

constexpr size_t TypedPropertyStorage::getStd140Alignment(PropertyType type) {
    switch (type) {
        case PropertyType::Float: case PropertyType::Int: 
        case PropertyType::Uint:  case PropertyType::Bool: return 4;
        case PropertyType::Vec2:  case PropertyType::IVec2: 
        case PropertyType::UVec2: return 8;
        case PropertyType::Vec3:  case PropertyType::IVec3: 
        case PropertyType::UVec3: case PropertyType::Vec4:  
        case PropertyType::IVec4: case PropertyType::UVec4:
        case PropertyType::Mat2:  case PropertyType::Mat3:  
        case PropertyType::Mat4: 
            return 16;
        default: return 4;
    }
}

constexpr size_t TypedPropertyStorage::getAlignment(PropertyType type) {
    return getStd140Alignment(type);
}

