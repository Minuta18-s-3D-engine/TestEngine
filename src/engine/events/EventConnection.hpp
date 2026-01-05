#ifndef ENGINE_EVENTS_EVENTCONNECTION_H_
#define ENGINE_EVENTS_EVENTCONNECTION_H_

#include <typeindex>

#include "Event.hpp"

class EventManager;

class EventConnection {
    friend class EventManager;

    int id = -1;
    bool valid = false;
    std::type_index type = typeid(void);

public:
    EventConnection(int _id, std::type_index _type) noexcept
        : id(_id), valid(true), type(_type) {}
    EventConnection() = default;
    ~EventConnection() = default;
    EventConnection(const EventConnection&) = delete;

    EventConnection(EventConnection&& other) noexcept
        : id(other.id), valid(other.valid), type(other.type) {
        other.valid = false;
    }

    EventConnection& operator=(EventConnection&& other) noexcept {
        if (this != &other) {
            id = other.id;
            valid = other.valid;
            type = other.type;
            other.valid = false;
        }
        return *this;
    }

    EventConnection& operator=(const EventConnection&) = delete;

    bool isConnected() const { return valid; }
    operator bool() const { return valid; }
};

#endif // ENGINE_EVENTS_EVENTCONNECTION_H_
