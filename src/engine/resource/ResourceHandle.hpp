#ifndef ENGINE_RESOURCE_RESOURCEHANDLE_HPP_
#define ENGINE_RESOURCE_RESOURCEHANDLE_HPP_

#include <cstdint>

using HandleID = uint32_t;

constexpr HandleID NULL_RESOURCE = 0;

template <typename T>
struct ResourceHandle final {
    HandleID id = NULL_RESOURCE;

    explicit ResourceHandle() : id(0) {}
    explicit ResourceHandle(const HandleID _id) : id(_id) {}
    
    [[nodiscard]] bool isValid() const { return (id != NULL_RESOURCE); }

    bool operator==(const ResourceHandle& other) const {
        return (id == other.id);
    }

    bool operator!=(const ResourceHandle& other) const {
        return (id != other.id);
    }

    static ResourceHandle<T> createNullHandle() {
        return ResourceHandle<T>(NULL_RESOURCE);
    }
};

#endif // ENGINE_RESOURCE_RESOURCEHANDLE_HPP_
