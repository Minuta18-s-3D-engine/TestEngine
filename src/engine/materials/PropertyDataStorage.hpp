#ifndef ENGINE_MATERIALS_PROPERTYDATASTORAGE_H_
#define ENGINE_MATERIALS_PROPERTYDATASTORAGE_H_

#include <stdexcept>

#include "MaterialLayout.hpp"
#include "MaterialDataBuffer.hpp"

class PropertyDataStorage {
    const MaterialLayout& layout;
    MaterialDataBuffer& buffer;

    uint32_t instanceId;

    // Obviously invalid id for move constructor
    static constexpr uint32_t INVALID_ID = 0xFFFFFFFF;
public:
    PropertyDataStorage(
        const MaterialLayout& _layout, 
        MaterialDataBuffer& _buffer
    );

    PropertyDataStorage(const PropertyDataStorage& other);
    PropertyDataStorage& operator=(const PropertyDataStorage& other);

    PropertyDataStorage(PropertyDataStorage&& other) noexcept;
    PropertyDataStorage& operator=(PropertyDataStorage&& other) noexcept;

    bool hasProperty(const std::string& name) const;

    template <typename T>
    void setProperty(const std::string& name, const T& value);

    template <typename T>
    T getProperty(const std::string& name) const;
};

template <typename T>
void PropertyDataStorage::setProperty(
    const std::string& name, const T& value
) {
    if (!hasProperty(name)) {
        throw std::invalid_argument("No such property: " + name);
    }

    const auto& info = layout.getPropertyInfo(name);
    if (MaterialLayout::getPropertyType<T>() != info.type) {
        throw std::invalid_argument(
            "Excepted type " + MaterialLayout::getGLSLType(info.type)
        );
    }

    buffer.write(instanceId, info.offset, info.size, &value);
}

template <typename T>
T PropertyDataStorage::getProperty(const std::string& name) const {
    if (!hasProperty(name)) {
        throw std::invalid_argument("No such property: " + name);
    }
    
    const auto& info = layout.getPropertyInfo(name);

    T value;
    buffer.read(instanceId, info.offset, sizeof(T), &value);
    return value;
}

#endif // ENGINE_MATERIALS_PROPERTYDATASTORAGE_H_
