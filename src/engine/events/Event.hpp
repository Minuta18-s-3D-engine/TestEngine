#ifndef ENGINE_EVENTS_EVENT_H_
#define ENGINE_EVENTS_EVENT_H_

class Event {
    bool handled = false;
public:
    bool isHandled() const { return handled; };
    void setIsHandled() { handled = true; }

    virtual ~Event() = default;
};

#endif // ENGINE_EVENTS_EVENT_H_
