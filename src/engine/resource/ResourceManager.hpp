#ifndef ENGINE_RESOURCE_RESOURCEMANAGER_HPP_
#define ENGINE_RESOURCE_RESOURCEMANAGER_HPP_

#include <unordered_map>
#include <typeindex>
#include <memory>

#include "ResourceHandle.hpp"
#include "engine/project/VirtualPath.hpp"
#include "engine/debug/logging/Logging.hpp"
#include "engine/utils/exc/GeneralExceptions.hpp"
#include "ResourceImporter.hpp"

class IResourceStorage {
public:
    virtual ~IResourceStorage() = default;
};

template <typename T>
class ResourceStorage : public IResourceStorage {
public:
    std::unordered_map<HandleID, T> storage;
};

class ResourceManager final {
    std::unordered_map<
        std::type_index, std::unique_ptr<IResourceStorage>
    > storages;
    std::unordered_map<std::string, HandleID> fileID;
    std::unordered_map<
        std::type_index, std::unique_ptr<IResourceImporter>
    > importers;

    Logger logger;

    HandleID nextID = ResourceHandle::NULL_RESOURCE + 1;

    template <typename T>
    ResourceStorage<T>& getOrCreateStorage();
public:
    ResourceManager();
    ~ResourceManager() = default;

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    template <typename T>
    ResourceHandle<T> load(const VirtualPath& path);

    template <typename T>
    [[nodiscard]] ResourceHandle<T> getByPath(const VirtualPath& path);

    template <typename T>
    [[nodiscard]] T* get(ResourceHandle<T> handle);

    template <typename T>
    [[nodiscard]] T& require(ResourceHandle<T> handle);

    template <typename T>
    void registerImporter(std::unique_ptr<ResourceImporter<T>> importer);
};

template <typename T>
ResourceStorage<T>& ResourceManager::getOrCreateStorage() {
    auto tid = std::type_index(typeid(T));
    auto it = storages.find(tid);
    if (it == storages.end()) {
        auto newStorage = std::make_unique<ResourceStorage<T>>();
        auto* storagePtr = newStorage.get();
        storages[tid] = std::move(newStorage);
        return *storagePtr; 
    }
    return *static_cast<ResourceStorage<T>*>(it->second.get());
}

template <typename T>
ResourceHandle<T> ResourceManager::load(const VirtualPath& path) {
    std::string strID = path.resolve();

    if (fileID.contains(strID)) {
        return ResourceHandle<T>(fileID[strID]);
    }

    auto tid = std::type_index(typeid(T));
    auto it = importers.find(tid);
    if (it == importers.end()) {
        logger.error(
            "No importer registered for type: " + std::string(typeid(T).name())
        );
        throw exc::invalid_argument(
            "No importer registered for this resource type"
        );
    }

    auto* typedImporter = static_cast<ResourceImporter<T>*>(it->second.get());

    T resource = typedImporter->import(path, *this);

    HandleID id = ++nextID;
    auto& s = getOrCreateStorage<T>();
    
    s.storage.emplace(id, std::move(resource));
    fileID[strID] = id;

    logger.info(
        "Loaded resource: " + strID + " with ID " + std::to_string(id)
    );

    return ResourceHandle<T>(id);
}

template <typename T>
ResourceHandle<T> ResourceManager::getByPath(const VirtualPath& path) {
    std::string strID = path.resolve();
    if (!fileID.contains(strID)) {
        throw exc::invalid_argument("No such resource: " + strID);
    }
    return ResourceHandle<T>(fileID[strID]);
}

template <typename T>
T* ResourceManager::get(ResourceHandle<T> handle) {
    if (!handle.isValid()) return nullptr;

    auto& s = getOrCreateStorage<T>();
    auto it = s.storage.find(handle.id);
    if (it != s.storage.end()) {
        return &it->second;
    } 

    return nullptr;
}

template <typename T>
T& ResourceManager::require(ResourceHandle<T> handle) {
    T* resource = get(handle);
    
    if (resource == nullptr) throw exc::invalid_argument("Invalid handle");

    return *resource;
}

template <typename T>
void ResourceManager::registerImporter(
    std::unique_ptr<ResourceImporter<T>> importer
) {
    auto tid = std::type_index(typeid(T));
    importers[tid] = std::move(importer);
}

#endif // ENGINE_RESOURCE_RESOURCEMANAGER_HPP_
