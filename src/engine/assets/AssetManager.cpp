#include "AssetManager.hpp"

template <typename T>
T* AssetManager::get(const std::string& name) const {
    const auto& mapIter = assets.find(typeid(T));
    if (item == assets.end()) return nullptr;
    const auto& map = mapIter->second;
    const auto& found = map.find(name);
    if (found == map.end()) return nullptr;
    return static_cast<T*>(found->second.get());
}
