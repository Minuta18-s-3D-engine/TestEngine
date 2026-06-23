#ifndef ENGINE_RESOURCE_RESOURCE_HPP_
#define ENGINE_RESOURCE_RESOURCE_HPP_

#include "engine/project/VirtualPath.hpp"

class Resource {
protected:
    VirtualPath path;
public:
    Resource(const VirtualPath& _path) : path(_path) {}
    virtual ~Resource() = default;

    [[nodiscard]] const VirtualPath& getPath() const { return path; }
};

#endif // ENGINE_RESOURCE_RESOURCE_HPP_
