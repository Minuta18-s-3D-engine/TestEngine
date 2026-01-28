#ifndef ENGINE_WINDOW_WINDOWEVENTS_H_
#define ENGINE_WINDOW_WINDOWEVENTS_H_

#include <cstdint>

#include "../events/Event.hpp"

class WindowResizeEvent : public Event {
public:
    uint32_t newWidth, newHeight;

    WindowResizeEvent(uint32_t _newWidth, uint32_t _newHeight) 
        : newWidth(_newWidth), newHeight(_newHeight) {}
};

#endif // ENGINE_WINDOW_WINDOWEVENTS_H_
