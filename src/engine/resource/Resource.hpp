#ifndef ENGINE_RESOURCE_RESOURCE_HPP_
#define ENGINE_RESOURCE_RESOURCE_HPP_

#include "engine/project/VirtualPath.hpp"

class Resource {
protected:
    VirtualPath path;
public:
    virtual ~Resource() = default;

    [[nodiscard]] const VirtualPath& getPath() const { return path; }
};

#endif // ENGINE_RESOURCE_RESOURCE_HPP_
