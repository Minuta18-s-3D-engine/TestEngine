#ifndef ENGINE_EVENTS_EVENTMANAGER_H_
#define ENGINE_EVENTS_EVENTMANAGER_H_

#include <functional>
#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>

class Event;

template <typename T>
concept DerivedEvent = std::is_base_of<Event, T>::value;

#include "Event.hpp"

template <typename T>
using Subscriber = std::function<void(const T&)>;

class EventHandlerInterface {
    virtual void call(const Event& event) = 0;
public:
    void exec(const Event& event) { call(event); }

    virtual ~EventHandlerInterface() = default;
};

template <DerivedEvent T>
class EventHandler : public EventHandlerInterface {
    Subscriber<T> func;

    void call(const Event& event) override {
        func(static_cast<const T&>(event));
    }
public:
    EventHandler(Subscriber<T> f) : func(f) {}
};

class EventManager {
private:
    std::vector<std::unique_ptr<Event>> eventQueue;
    std::unordered_map<std::type_index, 
        std::vector<std::unique_ptr<EventHandlerInterface>>> subscribers;
public:
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;

    template<DerivedEvent T>
    void subscribe(Subscriber<T> eventHandler);
    
    template<DerivedEvent T>
    void triggerEvent(const T& event);
    
    void dispatchEvents();
};

template<DerivedEvent T>
void EventManager::subscribe(Subscriber<T> eventHandler) {
    subscribers[typeid(T)].push_back(
        std::make_unique<EventHandler<T>>(eventHandler)
    );
}

template<DerivedEvent T>
void EventManager::triggerEvent(const T& event) {
    eventQueue.push_back(std::make_unique<T>(event));
}

#endif // ENGINE_EVENTS_EVENTMANAGER_H_
