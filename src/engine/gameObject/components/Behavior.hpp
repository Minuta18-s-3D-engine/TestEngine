#ifndef ENGINE_GAMEOBJECT_COMPONENTS_BEHAVIOR_H_
#define ENGINE_GAMEOBJECT_COMPONENTS_BEHAVIOR_H_

#include <memory>

#include "../Component.hpp"

enum BehaviorType {
    STATIC = 0,
    DYNAMIC = 1
};

class Behavior : public Component {
public:
    BehaviorType type = BehaviorType::DYNAMIC;

    std::unique_ptr<Component> clone() override {
        auto ptr = std::make_unique<Behavior>();
        ptr->type = type;
        return ptr;
    }
};

#endif // ENGINE_GAMEOBJECT_COMPONENTS_BEHAVIOR_H_