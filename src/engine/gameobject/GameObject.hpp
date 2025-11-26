#ifndef ENGINE_GAMEOBJECT_GAMEOBJECT_H_
#define ENGINE_GAMEOBJECT_GAMEOBJECT_H_

// some undocumented shit
// its required for stduuid to access uuids::uuid_system_generator
#define UUID_SYSTEM_GENERATOR

#include <uuid.h>

#include <vector>
#include <string>
#include <type_traits> 
#include <stdexcept>
#include <memory>
#include <unordered_map>
#include <typeindex>

#include "Component.hpp"

class GameObject {
    uuids::uuid id;
    std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
public:
    GameObject();
    GameObject(const GameObject& other);
    GameObject(GameObject&& other) noexcept;
    ~GameObject() = default;

    template<typename T>
    bool hasComponent() const;

    template<typename T>
    T* getComponent(bool exc = true) const;

    template<typename T>
    void addComponent(std::unique_ptr<T>& component, bool exc = true);

    template<typename T>
    void removeComponent();

    size_t getComponentCount() const;

    uuids::uuid getUuid() const;

    GameObject& operator=(const GameObject& other) noexcept;
    GameObject& operator=(GameObject&& other) noexcept;

    bool operator==(const GameObject& other) const noexcept { 
        return id == other.id; 
    }
};

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
T* GameObject::getComponent(bool exc) const {
    static_assert(
        std::is_base_of<Component, T>::value, "T must be Component"
    );

    for (auto& [index, comp] : components) {
        if (dynamic_cast<T*>(comp.get())) {
            return dynamic_cast<T*>(comp.get());
        }
    }

    if (exc) {
        throw std::invalid_argument("Component not found");
    } else {
        return nullptr;
    }
}

template<typename T>
void GameObject::addComponent(std::unique_ptr<T>& component, bool exc) {
    static_assert(
        std::is_base_of<Component, T>::value, "T must be Component"
    );

    if (exc) {
        if (components.count(typeid(component))) {
            throw std::invalid_argument("Component already exists");
        }
    }

    components[typeid(component)] = std::move(component);
}

template<typename T>
void GameObject::removeComponent() {
    static_assert(std::is_base_of_v<Component, T>, "T must be Component");
    
    components.erase(typeid(T));
}

#endif // ENGINE_GAMEOBJECT_GAMEOBJECT_H_
