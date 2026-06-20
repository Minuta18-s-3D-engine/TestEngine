#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define UUID_SYSTEM_GENERATOR

#include <uuid.h>

#include "engine/debug/logging/Logging.hpp"
#include "engine/debug/logging/ConsoleLoggerMiddleware.hpp"
#include "engine/debug/logging/FileLoggerMiddleware.hpp"
#include "engine/core/Application.hpp"
#include "engine/core/CommandLineArgs.hpp"

int main(int argc, char* argv[]) {
    try {
        Clock::initialize();

        Logging::setDefaultConfig(LoggerConfig::create(
            LogLevel::DEBUG,
            ConsoleLoggerMiddleware(),
            FileLoggerMiddleware("engine.log")
        ));
        Logger mainLogger = Logging::createLogger("engine.main");

        CommandLineArgs args(argc, argv);

        Application app(args);
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return -1;
    }
}
