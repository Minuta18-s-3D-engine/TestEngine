#ifndef ENGINE_SCENE_STATICOBJECT_H_
#define ENGINE_SCENE_STATICOBJECT_H_

#include "glm/glm.hpp"

#include "SceneObject.hpp"
#include "../graphics/Shader.hpp"

#include <iostream>

class StaticObject : public SceneObject {
public:
    StaticObject(std::shared_ptr<Model> _model, glm::vec3 _pos); 

    void draw(Shader& shader) override;
};

#endif // ENGINE_SCENE_STATICOBJECT_H_
