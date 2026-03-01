#ifndef ENGINE_GAMEOBJECT_COMPONENTS_HIERARCHY_H_
#define ENGINE_GAMEOBJECT_COMPONENTS_HIERARCHY_H_

#include <uuid.h>

#include <string>
#include <memory>

#include "../Component.hpp"

class Hierarchy : public Component {
public:
    std::string name = "";
    uuids::uuid parent;
    std::vector<uuids::uuid> children;

    std::unique_ptr<Component> clone() override {
        auto ptr = std::make_unique<Hierarchy>();
        ptr->name = name;
        ptr->parent = parent;
        ptr->children = children;
        return ptr;
    }
};

#endif // ENGINE_GAMEOBJECT_COMPONENTS_HIERARCHY_H_
