#ifndef ENGINE_MODELS_MODELCOMPONENT_H_
#define ENGINE_MODELS_MODELCOMPONENT_H_

#include <uuid.h>

#include "../gameobject/Component.hpp"

class ModelComponent : public Component {
public:
    uuids::uuid managerId;

    std::unique_ptr<Component> clone() override {
        auto ptr = std::make_unique<ModelComponent>();
        ptr->managerId = managerId;
        return ptr;
    }
};


#endif // ENGINE_MODELS_MODELCOMPONENT_H_

