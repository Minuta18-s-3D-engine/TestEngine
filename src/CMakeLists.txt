project(TestEngSrc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
add_library(glad ../lib/glad/include/glad/glad.h ../lib/glad/src/glad.c)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS} )

target_include_directories(glad PUBLIC ../lib/glad/include/)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(TestEngSrc ${OPENGL_LIBRARIES} glfw glad glm::glm)
