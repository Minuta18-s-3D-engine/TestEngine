#ifndef ENGINE_ASSETS_RESOURCEHANDLE_HPP_
#define ENGINE_ASSETS_RESOURCEHANDLE_HPP_

#include <cstdint>

using HandleID = uint32_t;

template <typename T>
struct ResourceHandle final {
    static const uint32_t NULL_RESOURCE = 0;

    uint32_t id = NULL_RESOURCE;

    ResourceHandle(uint32_t _id) : id(_id) {}
    
    bool isValid() { return (id != NULL_RESOURCE); }

    bool operator==(const ResourceHandle& other) const {
        return (id == other.id);
    }

    bool operator!=(const ResourceHandle& other) const {
        return (id != other.id);
    }
};

#endif // ENGINE_ASSETS_RESOURCEHANDLE_HPP_
