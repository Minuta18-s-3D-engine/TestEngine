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
    T* getComponent() const;

    template<typename T>
    void addComponent(std::unique_ptr<T> component, bool exc = true);

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

#endif // ENGINE_GAMEOBJECT_GAMEOBJECT_H_
