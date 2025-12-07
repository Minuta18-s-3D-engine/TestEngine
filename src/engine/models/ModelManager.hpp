#ifndef ENGINE_MODELS_MODELMANAGER_H_
#define ENGINE_MODELS_MODELMANAGER_H_

#define UUID_SYSTEM_GENERATOR
#include <uuid.h>

#include <unordered_map>
#include <memory>

#include "Model.hpp"

class ModelManager {
    std::unordered_map<uuids::uuid, std::unique_ptr<Model>> store;
public:
    ModelManager() = default;

    uuids::uuid add(Model* model);
    uuids::uuid add(std::unique_ptr<Model>& model);
    void set(uuids::uuid id, Model* model);
    void set(uuids::uuid id, std::unique_ptr<Model>& model);
    Model* get(uuids::uuid id);
};

#endif // ENGINE_MODELS_MODELMANAGER_H_
