#include "EventManager.hpp"

void EventManager::triggerEvent(std::unique_ptr<Event> event) {
    eventQueue.push_back(event);
}

void EventManager::dispatchEvents() {
    for (auto& event : eventQueue) {
        for (auto& handler : subscribers[typeid(*event.get())]) {
            handler(*event.get());
        }
    }
}
