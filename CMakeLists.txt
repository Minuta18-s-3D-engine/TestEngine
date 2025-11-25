cmake_minimum_required(VERSION 3.15)
project(TestEng)

option(BUILD_TESTS "Build unit tests") 

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(src)

add_executable(TestEng src/main.cpp)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} TestEngSrc ${CMAKE_DL_LIBS})

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
