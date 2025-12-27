#ifndef ENGINE_EVENTS_EVENTMANAGER_H_
#define ENGINE_EVENTS_EVENTMANAGER_H_

#include <functional>
#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>

#include "Event.hpp"

class EventManager {
public:
    using EventHandler = std::function<void(const Event&)>;
private:
    std::vector<std::unique_ptr<Event>> eventQueue;
    std::unordered_map<std::type_index, std::vector<EventHandler>> subscribers;
public:
    template<typename T>
    void subscribe(EventHandler eventHandler);
    
    void triggerEvent(std::unique_ptr<Event> event);
    void dispatchEvents();
};

template<typename T>
void EventManager::subscribe(EventManager::EventHandler eventHandler) {
    static_assert(std::is_base_of_v<Event, T>, "T must be Event");

    subscribers[typeid(T)].push_back(eventHandler);
}

#endif // ENGINE_EVENTS_EVENTMANAGER_H_
