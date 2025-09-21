#ifndef ENGINE_ASSETS_ASSETMANAGER_H_
#define ENGINE_ASSETS_ASSETMANAGER_H_

#include <unordered_map>
#include <string>
#include <memory>
#include <typeindex>

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

    template <typename T>
    T* get(const std::string& name) const;

    template <typename T>
    std::shared_ptr<T> getShared(const std::string& name) const;

    template <typename T>
    void set(std::unique_ptr<T> asset, const std::string& name);

    template <typename T>
    void set(std::shared_ptr<T> asset, const std::string& name);

    template <typename T>
    T& require(const std::string& name);
};

#endif // ENGINE_ASSETS_ASSETMANAGER_H_
