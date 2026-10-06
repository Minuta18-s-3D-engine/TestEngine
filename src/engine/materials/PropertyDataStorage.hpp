#ifndef ENGINE_MATERIALS_PROPERTYDATASTORAGE_H_
#define ENGINE_MATERIALS_PROPERTYDATASTORAGE_H_

#include <stdexcept>

#include <glm/gtc/type_ptr.hpp>

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

namespace shader_layout::detail {

template <typename T>
inline void writeStd430(
    MaterialDataBuffer& buf, uint32_t id, uint32_t off, const T& v
) {
    buf.write(id, off, sizeof(T), &v);
}

inline void writeStd430(
    MaterialDataBuffer& buf, uint32_t id, uint32_t off, bool v
) {
    const uint32_t packed = v ? 1u : 0u;
    buf.write(id, off, sizeof(uint32_t), &packed);
}

inline void writeStd430(
    MaterialDataBuffer& buf, uint32_t id, uint32_t off, const glm::mat3& v
) {
    for (int c = 0; c < 3; ++c) {
        buf.write(
            id, off + c * 16,
            sizeof(glm::vec3), glm::value_ptr(v[c])
        );
    }
}

template <typename T>
inline void readStd430(
    MaterialDataBuffer& buf, uint32_t id, uint32_t off, T& out
) {
    buf.read(id, off, sizeof(T), &out);
}

inline void readStd430(
    MaterialDataBuffer& buf, uint32_t id, uint32_t off, bool& out
) {
    uint32_t packed = 0;
    buf.read(id, off, sizeof(uint32_t), &packed);
    out = (packed != 0u);
}

inline void readStd430(
    MaterialDataBuffer& buf, uint32_t id, uint32_t off, glm::mat3& out
) {
    for (int c = 0; c < 3; ++c) {
        buf.read(
            id, off + c * 16, sizeof(glm::vec3),
            glm::value_ptr(out[c])
        );
    }
}

} // namespace shader_layout::detail

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
            "Type mismatch for property \"" + name + "\": expected " +
            shader_layout::typeInfo(property.type).glslName
        );
    }

    shader_layout::detail::writeStd430(
        *buffer, instanceId, property.offset, value
    );
}

template <typename T>
T PropertyDataStorage::getProperty(const std::string& name) const {
    if (!hasProperty(name)) {
        throw std::invalid_argument("No such property: " + name);
    }
    
    const auto& property = layout->getProperty(name);
    T value{};
    shader_layout::detail::readStd430(
        *buffer, instanceId, property.offset, value
    );
    return value;
}

#endif // ENGINE_MATERIALS_PROPERTYDATASTORAGE_H_
