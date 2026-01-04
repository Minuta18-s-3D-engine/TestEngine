#include "EventManager.hpp"

void EventManager::dispatchEvents() {
    for (auto& event : eventQueue) {
        auto it = subscribers.find(typeid(*event));
        if (it == subscribers.end()) continue;

        for (auto& handler : it->second) {
            handler->exec(*event);
        }
    }

    eventQueue.clear();
}
