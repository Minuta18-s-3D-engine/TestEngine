#include "ModelManager.hpp"

uuids::uuid ModelManager::add(Model* model) {
    auto id = uuids::uuid_system_generator{}();
    store[id] = std::unique_ptr<Model>(model);
    return id;
}

uuids::uuid ModelManager::add(std::unique_ptr<Model>& model) {
    auto id = uuids::uuid_system_generator{}();
    store[id] = std::move(model);
    return id;
}

void ModelManager::set(uuids::uuid id, Model* model) {
    store[id] = std::unique_ptr<Model>(model);
}

void ModelManager::set(uuids::uuid id, std::unique_ptr<Model>& model) {
    store[id] = std::move(model);
}

Model* ModelManager::get(uuids::uuid id) {
    return store[id].get();
}
