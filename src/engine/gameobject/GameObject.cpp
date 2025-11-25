#include "GameObject.hpp"

GameObject::GameObject() {
    id = uuids::uuid_system_generator{}();
}

GameObject::GameObject(const GameObject& other) {
    this->id = uuids::uuid_system_generator{}();
    for (const auto& [index, comp] : other.components) {
        components[typeid(comp)] = comp->clone();
    }
}

GameObject::GameObject(GameObject&& other) noexcept
    : id(std::move(other.id)),
      components(std::move(other.components)) {}

GameObject& GameObject::operator=(const GameObject& other) noexcept {
    if (this != &other) {
        this->id = uuids::uuid_system_generator{}();
        this->components.clear();
        for (const auto& [index, comp] : other.components) {
            components[typeid(comp)] = comp->clone();
        }
    }
    return *this;
}

GameObject& GameObject::operator=(GameObject&& other) noexcept {
    if (this != &other) {
        this->id = std::move(other.id);
        this->components = std::move(other.components);
    }
    return *this;
}

size_t GameObject::getComponentCount() const {
    return components.size();
}

uuids::uuid GameObject::getUuid() const {
    return id;
}

