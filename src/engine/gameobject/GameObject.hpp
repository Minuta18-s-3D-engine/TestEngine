#ifndef ENGINE_GAMEOBJECT_GAMEOBJECT_H_
#define ENGINE_GAMEOBJECT_GAMEOBJECT_H_

#include <uuid.h>

#include <vector>
#include <string>

#include "Component.hpp"

class GameObject {
    uuids::uuid id;
    std::string name;
public:
    std::vector<Component*> components;

    GameObject() = default;
    ~GameObject();
};

#endif ENGINE_GAMEOBJECT_GAMEOBJECT_H_
