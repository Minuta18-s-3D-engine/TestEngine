#ifndef ENGINE_SCENE_SCENE_H_
#define ENGINE_SCENE_SCENE_H_

#include "SceneObject.hpp"

#include <memory>
#include <vector>

class Scene {
    std::vector<std::shared_ptr<SceneObject>> objects;
public:
    Scene();

    void addObject(std::shared_ptr<SceneObject> obj);
    void drawAll(); // Move this to RenderSystem ad fast as possible
};

#endif // ENGINE_SCENE_SCENE_H_
