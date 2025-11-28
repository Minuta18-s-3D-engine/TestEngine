#include "GameObjectManager.hpp"

GameObjectManager::GameObjectManager() {}
GameObjectManager::~GameObjectManager() {}

void GameObjectManager::addObject(std::unique_ptr<GameObject>& gameObject) {
    objects[gameObject->getUuid()] = std::move(gameObject);
}

void GameObjectManager::addObject(GameObject* gameObject) {
    objects[gameObject->getUuid()] = std::unique_ptr<GameObject>(gameObject);
}

void GameObjectManager::removeObject(uuids::uuid id) {
    objects.erase(id);
}

GameObject* GameObjectManager::getObjectByUuid(uuids::uuid id) {
    return objects[id].get();
}

std::vector<GameObject*> GameObjectManager::findObjects(
    std::function<bool(GameObject*)> predicate) {
    std::vector<GameObject*> result;
    for (auto& [ key, value ] : objects) {
        if (predicate(value.get())) {
            result.push_back(value.get());
        }
    }

    return result;
}

GameObjectManager::iterator GameObjectManager::begin() {
    return objects.begin();
}

GameObjectManager::iterator GameObjectManager::end() {
    return objects.end();
}

size_t GameObjectManager::size() {
    return objects.size();
}
