#ifndef ENGINE_GAMEOBJECT_GAMEOBJECTMANAGER_H_
#define ENGINE_GAMEOBJECT_GAMEOBJECTMANAGER_H_

#include <uuid.h>
#include "glm/glm.hpp"

#include "GameObject.hpp"

#include <vector>
#include <unordered_map>
#include <memory>

class GameObjectManager {
private:
    std::unordered_map<uuids::uuid, std::unique_ptr<GameObject>> objects;
public:
    typedef std::unordered_map<uuids::uuid, 
        std::unique_ptr<GameObject>>::iterator iterator ;

    GameObjectManager();
    ~GameObjectManager();

    void addObject(std::unique_ptr<GameObject>& gameObject);
    void addObject(GameObject* gameObject);
    void removeObject(uuids::uuid id);

    GameObject* getObjectByUuid(uuids::uuid id);
    // TODO
    // std::vector<GameObject*> getObjectsByTag(const std::string& tag);
    // std::vector<GameObject*> getObjectsInRadius(
    //     const glm::vec3& center, float radius);
    std::vector<GameObject*> findObjects(std::function<bool(GameObject*)>);

    template<typename... Components>
    std::vector<GameObject*> getObjectsWithComponents();

    template<typename... Components>
    void forEach(std::function<void(GameObject*)> callback);

    iterator begin();
    iterator end();
    size_t size();
};

template<typename... Components>
std::vector<GameObject*> GameObjectManager::getObjectsWithComponents() {
    std::vector<GameObject*> result;

    for (auto& [ key, value ] : objects) {
        if ((value->template hasComponent<Components>() && ...)) {
            result.push_back(value.get());
        }
    }

    return result;
}

template<typename... Components>
void GameObjectManager::forEach(std::function<void(GameObject*)> callback) {
    for (auto& [ key, value ] : objects) {
        if ((value->template hasComponent<Components>() && ...)) {
            callback(value.get());
        }
    }
}

#endif // ENGINE_GAMEOBJECT_GAMEOBJECTMANAGER_H_
