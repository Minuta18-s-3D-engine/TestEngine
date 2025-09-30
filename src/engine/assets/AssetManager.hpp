#ifndef ENGINE_ASSETS_ASSETMANAGER_H_
#define ENGINE_ASSETS_ASSETMANAGER_H_

#include <unordered_map>
#include <string>
#include <memory>
#include <typeindex>
#include <exception>
#include <stdexcept>

// Used this for reference:
// https://github.com/MihailRis/voxelcore/blob/main/src/assets/Assets.hpp

class AssetManager {
    using AssetMap = std::unordered_map<std::string, std::shared_ptr<void>>;
    std::unordered_map<std::type_index, AssetMap> assets;
public:
    AssetManager() = default;
    ~AssetManager() = default;

    AssetManager(const AssetManager& other) = delete;
    AssetManager& operator=(const AssetManager& other) = delete;

    template <class T>
    T* get(const std::string& name) const {
        const auto& mapIter = assets.find(typeid(T));
        if (mapIter == assets.end()) {
            return nullptr;
        }
        const auto& map = mapIter->second;
        const auto& found = map.find(name);
        if (found == map.end()) {
            return nullptr;
        }
        return static_cast<T*>(found->second.get());
    }

    template <class T>
    std::shared_ptr<T> getShared(const std::string& name) const {
        const auto& mapIter = assets.find(typeid(T));
        if (mapIter == assets.end()) {
            return nullptr;
        }
        const auto& map = mapIter->second;
        const auto& found = map.find(name);
        if (found == map.end()) {
            return nullptr;
        }
        return std::static_pointer_cast<T>(found->second);
    }

    template <class T>
    void set(std::unique_ptr<T> asset, const std::string& name) {
        assets[typeid(T)][name].reset(asset.release());
    }

    template <class T>
    void set(std::shared_ptr<T> asset, const std::string& name) {
        std::cout << "AssetManager:set(): " << name << std::endl;
        assets[typeid(T)][name] = std::move(asset);
    }

    template <class T>
    T& require(const std::string& name) const {
        T* asset = get<T>(name);
        if (asset == nullptr) {
            throw std::runtime_error("Asset " + name + " not found");
        }
        return *asset;
    }
};

#endif // ENGINE_ASSETS_ASSETMANAGER_H_
