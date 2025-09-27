#include "StaticObject.hpp"

StaticObject::StaticObject(std::shared_ptr<Model> _model, glm::vec3 _pos) : 
    SceneObject(_model, _pos) {}

void StaticObject::draw(Shader& shader) {
    model->draw(shader);
}