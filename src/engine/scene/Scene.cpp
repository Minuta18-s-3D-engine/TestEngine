#include "Scene.hpp"

Scene::Scene() {}

void Scene::drawAll() {
    for (auto& object : this->objects) {
        // object.get()->draw();
    }
}

void Scene::addObject(std::shared_ptr<SceneObject> obj) {
    this->objects.push_back(obj);
}
