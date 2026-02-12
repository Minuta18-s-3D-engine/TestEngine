project(TestEngSrc)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmakeConfig.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmakeConfig.h"
)

include(FetchContent)

FetchContent_Declare(
  stduuid
  GIT_REPOSITORY https://github.com/mariusbancila/stduuid
  GIT_TAG        master
  SOURCE_DIR     ${CMAKE_SOURCE_DIR}/lib/stduuid 
)

FetchContent_MakeAvailable(stduuid)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/include.zip
    URL_HASH SHA256=b8cb0ef2dd7f57f18933997c9934bb1fa962594f701cd5a8d3c2c80541559372
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(json)

add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE ${json_SOURCE_DIR}/include)

set(ASSIMP_SOURCE_DIR ${CMAKE_SOURCE_DIR}/lib/assimp)
set(ASSIMP_BINARY_DIR ${CMAKE_BINARY_DIR}/_deps/assimp-build)

if(EXISTS ${ASSIMP_SOURCE_DIR}/CMakeLists.txt)
    message(STATUS "Using local Assimp from: ${ASSIMP_SOURCE_DIR}")
    
    FetchContent_Declare(
        assimp
        SOURCE_DIR ${ASSIMP_SOURCE_DIR}
        BINARY_DIR ${ASSIMP_BINARY_DIR}
    )
else()
    message(FATAL_ERROR "No assimp installation found.")
endif()

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Disable tests" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "Disable installation" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Disable tools" FORCE)

FetchContent_MakeAvailable(assimp)

file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
add_library(glad ../lib/glad/include/glad/glad.h ../lib/glad/src/glad.c)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS} ${PNG_INCLUDE_DIR} )

add_subdirectory(lib/bvh)

target_include_directories(glad PUBLIC ../lib/glad/include/)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

if (UNIX AND NOT APPLE)
    target_link_libraries(TestEngSrc uuid)
endif()

target_link_libraries(TestEngSrc ${OPENGL_LIBRARIES} glfw glad glm::glm 
    ${PNG_LIBRARY} assimp::assimp bvh stduuid ${JPEG_LIBRARIES} nlohmann_json)