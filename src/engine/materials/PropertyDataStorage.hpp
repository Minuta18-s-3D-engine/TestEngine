#ifndef ENGINE_MATERIALS_PROPERTYDATASTORAGE_H_
#define ENGINE_MATERIALS_PROPERTYDATASTORAGE_H_

#include "MaterialLayout.hpp"
#include "MaterialDataBuffer.hpp"

class PropertyDataStorage {
    const MaterialLayout& layout;
    MaterialDataBuffer& buffer;
public:
    PropertyDataStorage(
        const MaterialLayout& _layout, 
        MaterialDataBuffer& _buffer
    );

    bool hasProperty(const std::string& name);

    template <typename T>
    void setProperty(const std::string& name, const T& value);

    template <typename T>
    T getProperty(const std::string& name);
};

#endif // ENGINE_MATERIALS_PROPERTYDATASTORAGE_H_
