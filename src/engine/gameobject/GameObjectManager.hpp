#ifndef ENGINE_GAMEOBJECT_GAMEOBJECTMANAGER_H_
#define ENGINE_GAMEOBJECT_GAMEOBJECTMANAGER_H_

#include <uuid.h>

#include "GameObject.hpp"

#include <vector>
#include <unordered_map>

class GameObjectManager {
private:
    std::unordered_map<uuids::uuid, std::unique_ptr<GameObject>> objects;
    std::vector<GameObject*> staticObjects;
    std::vector<GameObject*> dynamicObjects;

public:
    GameObjectManager();
    ~GameObjectManager();


};

#endif // ENGINE_GAMEOBJECT_GAMEOBJECTMANAGER_H_
