#ifndef ENGINE_SCENE_SCENE_H_
#define ENGINE_SCENE_SCENE_H_

#include <string>
#include <memory>

#include "../gameobject/GameObjectManager.hpp"

class Scene {
    std::string name;

    std::unique_ptr<GameObjectManager> objectManager;
public:
    Scene(
        const std::string& _name, 
        std::unique_ptr<GameObjectManager> manager
    );
    ~Scene() = default;

    const std::string& getName();
    GameObjectManager& getGameObjectManager();
};

#endif // ENGINE_SCENE_SCENE_H_
