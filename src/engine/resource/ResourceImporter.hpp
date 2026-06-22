#ifndef ENGINE_RESOURCE_RESOURCEIMPORTER_HPP_
#define ENGINE_RESOURCE_RESOURCEIMPORTER_HPP_

#include <memory>

#include "Resource.hpp"

class ResourceManager;

class ResourceImporter {
public:
    virtual ~ResourceImporter() = default;

    virtual std::unique_ptr<Resource> importResource(
        const VirtualPath& path,
        const ResourceManager& manager
    ) = 0;
};

#endif // ENGINE_RESOURCE_RESOURCEIMPORTER_HPP_
