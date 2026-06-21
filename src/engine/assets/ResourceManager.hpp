#ifndef ENGINE_ASSETS_RESOURCEMANAGER_HPP_
#define ENGINE_ASSETS_RESOURCEMANAGER_HPP_

#include <unordered_map>
#include <typeindex>
#include <memory>

#include "ResourceHandle.hpp"
#include "engine/project/VirtualPath.hpp"

class IResourceStorage {
public:
    virtual ~IResourceStorage() = default;
};

template <typename T>
class ResourceStorage {
public:
    std::unordered_map<HandleID, T> storage;
};

class ResourceManager {
    std::unordered_map<
        std::type_index, std::unique_ptr<IResourceStorage>
    > storages;
    std::unordered_map<std::string, HandleID> fileID;

};

#endif // ENGINE_ASSETS_RESOURCEMANAGER_HPP_
