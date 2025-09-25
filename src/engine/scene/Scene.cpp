#include "Scene.hpp"

Scene::Scene(AssetManager& _assetManager) : assetManager(_assetManager) {}

void Scene::drawAll() {
    Shader* mainShader = assetManager.get<Shader>("shaders/main");

    for (auto& object : this->objects) {
        object.get()->draw(*mainShader);
    }
}

void Scene::addObject(std::shared_ptr<SceneObject> obj) {
    this->objects.push_back(obj);
}
