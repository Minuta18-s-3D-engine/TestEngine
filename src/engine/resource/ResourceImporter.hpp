#ifndef ENGINE_RESOURCE_RESOURCEIMPORTER_HPP_
#define ENGINE_RESOURCE_RESOURCEIMPORTER_HPP_

#include <memory>

class ResourceManager;

class IResourceImporter {
public:
    virtual ~IResourceImporter() = default;
};

template <typename T>
class ResourceImporter : public IResourceImporter {
public:
    virtual ~ResourceImporter() = default;

    virtual T importResource(
        const VirtualPath& path, const ResourceManager& manager
    ) = 0;
};

#endif // ENGINE_RESOURCE_RESOURCEIMPORTER_HPP_
