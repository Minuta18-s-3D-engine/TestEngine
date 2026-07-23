#ifndef ENGINE_MODELS_MODEL_H_
#define ENGINE_MODELS_MODEL_H_

#include <vector>
#include <memory>

#include "Mesh.hpp"
#include "../materials/Material.hpp"
#include "engine/resource/ResourceHandle.hpp"
#include "engine/resource/ResourceManager.hpp"

class ModelLoader;

class Model {
    ResourceManager* resourceManager;
public:
    friend class ModelLoader;

    using MeshArray = std::vector<ResourceHandle<Mesh>>;

    ResourceHandle<Material> material;
    MeshArray meshes;

    Model(
        ResourceManager& resourceManager_,
        MeshArray meshes_
    );

    void draw() const;
};

#endif // ENGINE_MODELS_MODEL_H_
