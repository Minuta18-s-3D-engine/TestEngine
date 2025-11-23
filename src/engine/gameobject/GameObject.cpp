#include "GameObject.hpp"

GameObject::~GameObject() {
    for (int i = 0; i < components.size(); ++i) {
        delete components[i];
    }
}
