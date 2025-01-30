cmake_minimum_required(VERSION 3.15)
project(TestEng)

set(CMAKE_CXX_STANDARD 17)

add_executable(TestEng src/main.cpp)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_library(glad lib/glad/include/glad/glad.h lib/glad/src/glad.c)

target_include_directories(glad PUBLIC lib/glad/include/)
include_directories( ${OPENGL_INCLUDE_DIRS} )
target_link_libraries(TestEng ${OPENGL_LIBRARIES} glfw glad)
