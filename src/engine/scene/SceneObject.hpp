#ifndef ENGINE_SCENE_SCENEOBJECT_H_
#define ENGINE_SCENE_SCENEOBJECT_H_

#include "glm/glm.hpp"

#include "../models/Model.hpp"
#include "../graphics/Shader.hpp"

#include <memory>

class SceneObject {
protected:
    std::shared_ptr<Model> model;
public:
    glm::vec3 position;
    // For future
    // glm::vec3 rotation, scale;
    
    SceneObject(std::shared_ptr<Model> _model, glm::vec3 _pos);

    virtual void draw(Shader& shader) = 0;
};

#endif // ENGINE_SCENE_SCENEOBJECT_H_
