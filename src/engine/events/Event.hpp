#ifndef ENGINE_EVENTS_EVENT_H_
#define ENGINE_EVENTS_EVENT_H_

class Event {
    bool handled = false;

    
    virtual ~Event();
};

#endif // ENGINE_EVENTS_EVENT_H_
