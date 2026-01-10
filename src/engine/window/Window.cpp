#include "Window.hpp"

const uint32_t DEFAULT_WIDTH = 1920;
const uint32_t DEFAULT_HEIGHT = 1080;
const std::string DEFAULT_CAPTION = "TestEng";

const int OPENGL_VERSION_MAJOR = 4;
const int OPENGL_VERSION_MINOR = 3;

Window::Window(
    uint32_t _width, uint32_t _height, const std::string& _caption, 
    EventManager& _eventManager
) : width(_width), height(_height), caption(_caption), 
    eventManager(_eventManager) {}

Window::Window(EventManager& _eventManager)
    : width(DEFAULT_WIDTH), height(DEFAULT_HEIGHT), caption(DEFAULT_CAPTION), 
    eventManager(_eventManager) {}

Window::~Window() {
    glfwSetWindowUserPointer(window, nullptr);
}

void Window::framebufferSizeCallback(
    GLFWwindow* _window, int _width, int _height
) {
    glViewport(0, 0, _width, _height);
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(_window));
    self->width = _width;
    self->height = _height;

    self->eventManager.triggerEvent(WindowResizeEvent(_width, _height));
}

void Window::setupWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(
        width, height, caption.c_str(), nullptr, nullptr
    );

    if (window == nullptr) {
        throw std::runtime_error("Failed to create GLFW Window");
    }
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)  glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }
    glViewport(0, 0, width, height);

    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, &Window::framebufferSizeCallback);
    
    glfwSwapInterval(0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    this->setCursorInputMode(GLFW_CURSOR_DISABLED);

    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    std::cout << "OpenGL Version: " << version << std::endl;
    std::cout << "GLSL Version: " << glslVersion << std::endl;
}

bool Window::isShouldClose() const {
    return glfwWindowShouldClose(window);
}

void Window::setShouldClose(bool should) {
    glfwSetWindowShouldClose(window, should);
}

std::string Window::getCaption() const {
    return caption;
}

void Window::setCaption(const std::string& _caption) {
    caption = _caption;
    glfwSetWindowTitle(window, _caption.c_str());
}

uint32_t Window::getHeight() const {
    return height;
}

uint32_t Window::getWidth() const {
    return width;
}

void Window::setViewport(uint32_t _width, uint32_t _height) {
    width = _width;
    height = _height;
    glViewport(0, 0, width, height);
}

int Window::getCursorInputMode() const {
    return cursorInputMode;
}

void Window::setCursorInputMode(int mode) {
    cursorInputMode = mode;
    glfwSetInputMode(window, GLFW_CURSOR, mode);
}

void Window::swapBuffers() {
    glfwSwapBuffers(window);
}
