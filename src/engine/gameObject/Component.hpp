#ifndef ENGINE_GAMEOBJECT_COMPONENT_H_
#define ENGINE_GAMEOBJECT_COMPONENT_H_

#include <memory>

class Component {
public:
    Component() = default;
    virtual ~Component() = default;
    
    virtual std::unique_ptr<Component> clone() = 0;
};

#endif // ENGINE_GAMEOBJECT_COMPONENT_H_