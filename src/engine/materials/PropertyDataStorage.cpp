#include "PropertyDataStorage.hpp"

PropertyDataStorage::PropertyDataStorage(
    const MaterialLayout& _layout,
    MaterialDataBuffer& _buffer
) : layout(_layout), buffer(_buffer) {
    if (!layout.isFinalized()) {
        throw std::invalid_argument("Layout must be finalized");
    }

    instanceId = buffer.allocateBlock(layout.getLayoutSize());
}

bool PropertyDataStorage::hasProperty(const std::string& name) {
    return layout.hasProperty(name);
}

PropertyDataStorage::PropertyDataStorage(const PropertyDataStorage& other)
    : layout(other.layout), buffer(other.buffer) {
    instanceId = buffer.allocateBlock(layout.getLayoutSize());

    std::vector<uint8_t> temp(layout.getLayoutSize());
    buffer.read(other.instanceId, 0, temp.size(), temp.data());
    buffer.write(instanceId, 0, temp.size(), temp.data());
}

PropertyDataStorage& PropertyDataStorage::operator=(
    const PropertyDataStorage& other
) {
    if (this == &other) return *this;
    if (&layout != &other.layout) 
        throw std::invalid_argument("Layouts not match");
    if (&buffer != &other.buffer)
        throw std::invalid_argument("buffers not match");
    
    std::vector<uint8_t> temp(layout.getLayoutSize());
    buffer.read(other.instanceId, 0, temp.size(), temp.data());
    buffer.write(instanceId, 0, temp.size(), temp.data());

    return *this;
}

PropertyDataStorage::PropertyDataStorage(PropertyDataStorage&& other) noexcept
    : layout(other.layout), buffer(other.buffer), 
    instanceId(other.instanceId) {
    other.instanceId = INVALID_ID;
}

PropertyDataStorage& PropertyDataStorage::operator=(
    PropertyDataStorage&& other
) noexcept {
    if (this == &other) return *this;

    instanceId = other.instanceId;
    other.instanceId = INVALID_ID;

    return *this;
}
