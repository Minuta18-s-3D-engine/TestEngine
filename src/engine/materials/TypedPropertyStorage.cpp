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
    uint8_t* newData = new uint8_t[n];
    for (int i = 0; i < std::min(n, dataSize); ++i) {
        newData[i] = data[i];
    }
    freeData();
    dataSize = n;
    data = newData;
}

void TypedPropertyStorage::freeData() {
    if (dataSize > 0) {
        delete[] data;
    }
}

bool TypedPropertyStorage::hasProperty(const std::string& name) {
    return properties.contains(name);
}

bool TypedPropertyStorage::isPropertySet(const std::string& name) {
    if (!hasProperty(name)) {
        throw std::invalid_argument("No such property: " + name);
    }
    return properties[name].isDefaultDataSet;
}

