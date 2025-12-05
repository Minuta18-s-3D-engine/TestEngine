#ifndef ENGINE_SCENE_SCENE_H_
#define ENGINE_SCENE_SCENE_H_

#include "../gameobject/GameObjectManager.hpp"

class Scene {
    GameObjectManager& objectManager;
public:
    Scene(GameObjectManager& manager);
    ~Scene() = default;

    GameObjectManager& getGameObjectManager();
};

#endif // ENGINE_SCENE_SCENE_H_
