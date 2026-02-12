#include "Scene.hpp"

Scene::Scene(
    const std::string& _name, std::unique_ptr<GameObjectManager> manager
) : objectManager(std::move(manager)), name(_name) {}

GameObjectManager& Scene::getGameObjectManager() {
    return *objectManager;
}

const std::string& Scene::getName() {
    return name;
}
