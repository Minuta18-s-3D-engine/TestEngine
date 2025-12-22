#ifndef ENGINE_EVENTS_EVENTMANAGER_H_
#define ENGINE_EVENTS_EVENTMANAGER_H_

class EventManager {
    void subscribe();
    void unsubscribe();
    void trigggerEvent();
    void dispatchEvents();
};

#endif // ENGINE_EVENTS_EVENTMANAGER_H_
