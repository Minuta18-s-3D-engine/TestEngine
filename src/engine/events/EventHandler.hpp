#ifndef ENGINE_EVENTS_EVENTHANDLER_H_
#define ENGINE_EVENTS_EVENTHANDLER_H_

#include <typeindex>
#include <cstddef>
#include <functional>
#include <memory>

class Event;

template <typename T>
concept DerivedEvent = std::is_base_of<Event, T>::value;

template <typename T>
using Subscriber = std::function<void(T&)>;

class EventHandlerInterface {
    virtual void call(Event& event) = 0;

    int id;
public:   
    EventHandlerInterface(int _id) : id(_id) {}

    void exec(Event& event) { call(event); }
    int getId() const { return id; }

    virtual ~EventHandlerInterface() = default;
};

template <DerivedEvent T>
class EventHandler : public EventHandlerInterface {
    Subscriber<T> func;

    void call(Event& event) override {
        func(static_cast<T&>(event));
    }

public:
    EventHandler() = default;
    EventHandler(Subscriber<T> f, int _id) : EventHandlerInterface(_id), func(std::move(f)) {}
};

#endif // ENGINE_EVENTS_EVENTHANDLER_H_
