#ifndef ENGINE_ASSETS_ASSETMANAGER_H_
#define ENGINE_ASSETS_ASSETMANAGER_H_

#include <unordered_map>
#include <string>
#include <memory>

enum class AssetType {
    TEXTURE,
    SHADER,
    MODEL
};

class AssetManager {
    using AssetMap = std::unordered_map<std::string, std::shared_ptr<void>>;
    std::unordered_map<AssetType, AssetMap> assets;
public:
    AssetManager() = default;
    ~AssetManager() = default;

    AssetManager(const AssetManager& other) = delete;
    AssetManager& operator=(const AssetManager& other) = delete;

};

#endif // ENGINE_ASSETS_ASSETMANAGER_H_
