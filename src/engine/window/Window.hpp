#ifndef ENGINE_GRAPHICS_WINDOW_H_
#define ENGINE_GRAPHICS_WINDOW_H_

#include <string>

class Window {
    std::string caption;
    uint32_t width, height;
    int cursorInputMode;
public:
    Window(uint32_t _width, uint32_t _height, std::string _caption);

    void close();

    std::string getCaption();
    void setCaption(std::string _caption);
    uint32_t getHeight();
    uint32_t getWidth();
    void setViewport(uint32_t _width, uint32_t _height);

    void swapBuffers();
};

#endif // ENGINE_GRAPHICS_WINDOW_H_