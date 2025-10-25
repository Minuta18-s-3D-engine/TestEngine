#include "Window.hpp"
#include "UserInput.hpp"

GLFWwindow* Window::window = nullptr;
int Window::width = 0;
int Window::height = 0;
std::string Window::caption = "TestEng";
int Window::cursorInputMode = GLFW_CURSOR_DISABLED;
std::vector<Window::FramebufferCallback> Window::framebufferSizeCallbacks;

void framebufferSizeCallback(GLFWwindow* win, int width, int height) {
    glViewport(0, 0, width, height);
    Window::width = width;
    Window::height = height;

    for (auto& callback : Window::framebufferSizeCallbacks) {
        callback(win, width, height);
    }
}

void Window::checkGlad() {
    if (!gladLoadGLLoader((GLADloadproc)  glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        throw std::runtime_error("Failed to initialize GLAD");
    }
}

void Window::initialize(
    uint width, uint height, 
    std::string caption
) {
    Window::width = width;
    Window::height = height;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window::window = glfwCreateWindow(
        width, height, caption.c_str(), nullptr, nullptr
    );

    if (Window::window == nullptr) {
        std::cerr << "Failed to create GLFW Window" << std::endl;
        throw std::runtime_error("Failed to create GLFW Window");
    }
    glfwMakeContextCurrent(Window::window);
    
    Window::checkGlad();

    glViewport(0, 0, Window::width, Window::height);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    glfwSetKeyCallback(Window::window, key_callback);
    glfwSetMouseButtonCallback(Window::window, mouse_button_callback);
    glfwSetCursorPosCallback(Window::window, cursor_position_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    
    glEnable(GL_DEPTH_TEST);

    Window::setCursorInputMode(GLFW_CURSOR_DISABLED);

    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    std::cout << "OpenGL Version: " << version << std::endl;
    std::cout << "GLSL Version: " << glslVersion << std::endl;
}

void Window::terminate() {
    glfwTerminate();
}

bool Window::isShouldClose() {
    return glfwWindowShouldClose(window);
}

void Window::setShouldClose(bool should_close) {
    glfwSetWindowShouldClose(window, should_close);
}

void Window::clearColor(glm::vec3 color) {
    glClearColor(color.r, color.g, color.b, 1.0f);
}

void Window::clearColor(glm::vec4 color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

void Window::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::setTitle(std::string newTitle) {
    glfwSetWindowTitle(Window::window, newTitle.c_str());
}

void Window::swapBuffers() {
    glfwSwapBuffers(Window::window);
}

void Window::setViewport(int x, int y, int width, int height) {
    glViewport(0, 0, width, height);
    Window::width = width;
    Window::height = height;
}

void Window::setCursorInputMode(int mode) {
    Window::cursorInputMode = mode;
    glfwSetInputMode(Window::window, GLFW_CURSOR, mode);  
}

int Window::getCursorInputMode() {
    return Window::cursorInputMode;
}

void Window::addframebufferCallback(FramebufferCallback callback) {
    Window::framebufferSizeCallbacks.push_back(callback);
}
