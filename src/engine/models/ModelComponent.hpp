#ifndef ENGINE_MODELS_MODELCOMPONENT_H_
#define ENGINE_MODELS_MODELCOMPONENT_H_

#include "../gameObject/Component.hpp"
#include "engine/resource/ResourceHandle.hpp"
#include "engine/models/Model.hpp"

class ModelComponent : public Component {
public:
    explicit ModelComponent(const  ResourceHandle<Model> resourceHandle)
        : modelHandle(resourceHandle) {};

    ResourceHandle<Model> modelHandle;

    std::unique_ptr<Component> clone() override {
        auto ptr = std::make_unique<ModelComponent>(modelHandle);
        return ptr;
    }
};


#endif // ENGINE_MODELS_MODELCOMPONENT_H_

