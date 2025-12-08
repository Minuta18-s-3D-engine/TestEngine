#ifndef ENGINE_MODELS_MODEL_H_
#define ENGINE_MODELS_MODEL_H_

#include <vector>
#include <string>
#include <memory>
#include <iostream>

#include "Mesh.hpp"

class ModelLoader;

class Model {
public:
    std::vector<std::shared_ptr<Mesh>> meshes;
    Model(std::vector<std::shared_ptr<Mesh>>& meshes);

    void draw(Shader& shader);

    friend class ModelLoader;
};

#endif // ENGINE_MODELS_MODEL_H_
