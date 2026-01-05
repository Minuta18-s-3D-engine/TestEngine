#include "EventManager.hpp"

void EventManager::dispatchEvents() {
    for (auto& event : eventQueue) {
        auto it = subscribers.find(typeid(*event));
        if (it == subscribers.end()) continue;

        for (auto& handler : it->second) {
            handler->exec(*event);
            if (event->isHandled()) break;
        }
    }

    eventQueue.clear();
}

void EventManager::unsubscribe(EventConnection& conn) {
    if (!conn.valid) return;

    auto it = subscribers.find(conn.type);
    if (it == subscribers.end()) return;
    
    auto& handlers = it->second;
    auto handlersIt = std::find_if(handlers.begin(), handlers.end(), 
        [&conn] (const std::unique_ptr<EventHandlerInterface>& handler) {
            return handler->getId() == conn.id;
        });

    if (handlersIt != handlers.end()) {
        handlers.erase(handlersIt);
        if (handlers.empty()) {
            subscribers.erase(it);
        }
    }

    conn.valid = false;
}
