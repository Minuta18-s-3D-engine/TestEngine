#include "AssetManager.hpp"

template <class T>
T* AssetManager::get(const std::string& name) const {
    const auto& mapIter = assets.find(typeid(T));
    if (item == assets.end()) {
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
std::shared_ptr<T> AssetManager::getShared(const std::string& name) const {
    const auto& mapIter = assets.find(typeid(T));
    if (item == assets.end()) {
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
void AssetManager::set(std::unique_ptr<T> asset, const std::string& name) {
    assets[typeid(T)][name].reset(asset.release());
}

template <class T>
void AssetManager::set(std::shared_ptr<T> asset, const std::string& name) {
    assets[typeid(T)][name] = std::move(asset);
}

template <class T>
T& AssetManager::require(const std::string& name) const {
    T* asset = get<T>(name);
    if (asset == nullptr) {
        throw std::runtime_error("Asset " + name + " not found");
    }
    return *asset;
}

