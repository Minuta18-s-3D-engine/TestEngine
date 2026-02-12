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
        std::unique_ptr<GameObjectManager> manager,
        const std::string& _name
    );
    ~Scene() = default;

    const std::string& getName();
    GameObjectManager& getGameObjectManager();
};

#endif // ENGINE_SCENE_SCENE_H_
