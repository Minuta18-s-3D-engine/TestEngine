#include "Scene.hpp"

Scene::Scene(
    std::unique_ptr<GameObjectManager> manager, const std::string& _name
) : objectManager(std::move(manager)), name(_name) {}

GameObjectManager& Scene::getGameObjectManager() {
    return *objectManager;
}

const std::string& Scene::getName() {
    return name;
}
