#include "GameObject.hpp"

GameObject::GameObject() {
    id = uuids::uuid_system_generator{}();
}

GameObject::GameObject(const GameObject& other) {
    this->id = uuids::uuid_system_generator{}();
    for (const auto& [index, comp] : other.components) {
        components[typeid(comp)] = 
            std::make_unique<Component>(comp->clone());
    }
}

GameObject::GameObject(GameObject&& other)
    : id(std::move(other.id)),
      components(std::move(other.components)) {}

GameObject& GameObject::operator=(const GameObject& other) noexcept {
    if (this != &other) {
        this->id = uuids::uuid_system_generator{}();
        this->components.clear();
        for (const auto& [index, comp] : other.components) {
            components[typeid(comp)] = 
                std::make_unique<Component>(comp->clone());
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

template<typename T>
bool GameObject::hasComponent() const {
    static_assert(
        std::is_base_of<Component, T>::value, "T must be Component"
    );

    for (const auto& [index, comp] : components) {
        if (dynamic_cast<T*>(comp.get())) {
            return true;
        }
    }

    return false;
}

template<typename T>
T* GameObject::getComponent() const {
    static_assert(
        std::is_base_of<Component, T>::value, "T must be Component"
    );

    for (const auto& [index, comp] : components) {
        if (dynamic_cast<T*>(comp.get())) {
            return components[comp].get();
        }
    }

    throw std::invalid_argument("Component not found");
}

template<typename T>
void addComponent(std::unique_ptr<T> component, bool exc) {
    static_assert(
        std::is_base_of<Component, T>::value, "T must be Component"
    );

    if (exc) {
        if (components.count(typeid(component))) {
            throw std::invalid_argument("Component already exists");
        }
    }

    components.push_back(component);
}

template<typename T>
void GameObject::removeComponent() {
    static_assert(std::is_base_of_v<Component, T>, "T must be Component");
    
    components.erase(typeid(T));
}

size_t GameObject::getComponentCount() const {
    return components.size();
}

uuids::uuid GameObject::getUuid() const {
    return id;
}

