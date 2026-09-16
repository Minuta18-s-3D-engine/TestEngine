#ifndef ENGINE_MATERIALS_PROPERTYDATASTORAGE_H_
#define ENGINE_MATERIALS_PROPERTYDATASTORAGE_H_

#include <stdexcept>

#include "MaterialDataBuffer.hpp"
#include "engine/graphics/ShaderLayout.hpp"

class PropertyDataStorage {
    const ShaderLayout* layout;
    MaterialDataBuffer* buffer;

    // Obviously invalid id for move constructor
    static constexpr uint32_t INVALID_ID = 0xFFFFFFFF;

    uint32_t instanceId = INVALID_ID;
public:
    explicit PropertyDataStorage(
        const ShaderLayout& _layout,
        MaterialDataBuffer& _buffer
    );
    explicit PropertyDataStorage(
        const PropertyDataStorage& other, 
        MaterialDataBuffer& targetBuffer
    );

    PropertyDataStorage(const PropertyDataStorage& other);
    PropertyDataStorage& operator=(const PropertyDataStorage& other);

    PropertyDataStorage(PropertyDataStorage&& other) noexcept;
    PropertyDataStorage& operator=(PropertyDataStorage&& other) noexcept;

    [[nodiscard]] bool hasProperty(const std::string& name) const;

    template <typename T>
    void setProperty(const std::string& name, const T& value);

    template <typename T>
    T getProperty(const std::string& name) const;

    [[nodiscard]] uint32_t getStartId() const {
        return buffer->getMetadataById(instanceId).offset;
    }

    [[nodiscard]] MaterialDataBuffer& getBuffer() const { return *buffer; }
    [[nodiscard]] const ShaderLayout& getLayout() const { return *layout; }

    void bindLayout(const ShaderLayout& newLayout) {
        layout = &newLayout;
    }
};

template <typename T>
void writeStd430(
    MaterialDataBuffer& buf, const uint32_t id, const uint32_t offset, const T& v
) {
    buf.write(id, offset, sizeof(T), &v);
}

template <typename T>
void readStd430(
    MaterialDataBuffer& buf, const uint32_t id, const uint32_t offset, T& out
) {
    buf.read(id, offset, sizeof(T), &out);
}

template <typename T>
void PropertyDataStorage::setProperty(
    const std::string& name, const T& value
) {
    if (!hasProperty(name)) {
        throw std::invalid_argument("No such property: " + name);
    }

    const auto& property = layout->getProperty(name);
    if (shader_layout::propertyTypeOf<T> != property.type) {
        throw std::invalid_argument(
            "Type mismatch for" + name +
                "Excepted type " + shader_layout::typeInfo(property.type).glslName
        );
    }

    writeStd430(buffer, instanceId, property.offset, value);
}

template <typename T>
T PropertyDataStorage::getProperty(const std::string& name) const {
    if (!hasProperty(name)) {
        throw std::invalid_argument("No such property: " + name);
    }
    
    const auto& property = layout->getProperty(name);
    T value{};
    readStd430(buffer, instanceId, property.offset, value);
    return value;
}

#endif // ENGINE_MATERIALS_PROPERTYDATASTORAGE_H_
