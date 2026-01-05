#ifndef ENGINE_EVENTS_EVENTMANAGER_H_
#define ENGINE_EVENTS_EVENTMANAGER_H_

#include <functional>
#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>
#include <algorithm>

#include "EventConnection.hpp"
#include "Event.hpp"
#include "EventHandler.hpp"

class EventManager {
private:
    std::vector<std::unique_ptr<Event>> eventQueue;
    std::unordered_map<std::type_index, 
        std::vector<std::unique_ptr<EventHandlerInterface>>> subscribers;

    // -1 so ids will start from 0
    int nextConnectionId = -1;
public:
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;

    template<DerivedEvent T>
    EventConnection subscribe(Subscriber<T> eventHandler);

    void unsubscribe(EventConnection& conn);

    template<DerivedEvent T>
    void clearSubscribers();
    
    void clearAllSubscribers();

    template<DerivedEvent T>
    void triggerEvent(const T& event);
    
    void dispatchEvents();
};

template<DerivedEvent T>
EventConnection EventManager::subscribe(Subscriber<T> eventHandler) {
    nextConnectionId++;

    subscribers[typeid(T)].push_back(
        std::make_unique<EventHandler<T>>(eventHandler, nextConnectionId)
    );

    return EventConnection(nextConnectionId, typeid(T));
}

template<DerivedEvent T>
void EventManager::triggerEvent(const T& event) {
    eventQueue.push_back(std::make_unique<T>(event));
}

template<DerivedEvent T>
void EventManager::clearSubscribers() {
    subscribers.erase(typeid(T));
}

void EventManager::clearAllSubscribers() {
    subscribers.clear();
}

#endif // ENGINE_EVENTS_EVENTMANAGER_H_
