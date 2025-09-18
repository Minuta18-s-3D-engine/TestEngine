#ifndef ENGINE_MODELS_MODEL_H_
#define ENGINE_MODELS_MODEL_H_

#include <vector>
#include <string>

#include "Mesh.hpp"

class Model {
    std::vector<Mesh> meshes;
    std::string filename;

    
public:
    Model(/* args */);
};

#endif // ENGINE_MODELS_MODEL_H_
