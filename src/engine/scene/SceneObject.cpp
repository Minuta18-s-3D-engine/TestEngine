#include "SceneObject.hpp"

SceneObject::SceneObject(std::shared_ptr<Model> _model, glm::vec3 _pos) :
    model(_model), position(_pos) {}
