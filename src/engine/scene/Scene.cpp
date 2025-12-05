#include "Scene.hpp"

Scene::Scene(GameObjectManager& manager) : objectManager(manager) {}

GameObjectManager& Scene::getGameObjectManager() {
    return objectManager;
}

