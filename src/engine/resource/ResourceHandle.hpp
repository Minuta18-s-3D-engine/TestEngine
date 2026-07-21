#ifndef ENGINE_RESOURCE_RESOURCEHANDLE_HPP_
#define ENGINE_RESOURCE_RESOURCEHANDLE_HPP_

#include <cstdint>

using HandleID = uint32_t;

constexpr uint32_t NULL_RESOURCE = 0;

template <typename T>
struct ResourceHandle final {
    uint32_t id = NULL_RESOURCE;

    explicit ResourceHandle(const uint32_t _id) : id(_id) {}
    
    [[nodiscard]] bool isValid() const { return (id != NULL_RESOURCE); }

    bool operator==(const ResourceHandle& other) const {
        return (id == other.id);
    }

    bool operator!=(const ResourceHandle& other) const {
        return (id != other.id);
    }
};

#endif // ENGINE_RESOURCE_RESOURCEHANDLE_HPP_
