set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(PNG REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS} ${PNG_INCLUDE_DIR} )

add_executable(engineTests
    main.cpp
    ${TEST_SOURCES}
)

target_link_libraries(engineTests PRIVATE 
    TestEngSrc       
    GTest::gtest_main  
    
    ${OPENGL_LIBRARIES}
    glfw
    glad 
    glm::glm
    ${PNG_LIBRARY}
    assimp::assimp
    bvh
    stduuid
    ${CMAKE_DL_LIBS}
)

if (UNIX AND NOT APPLE)
    target_link_libraries(engineTests PRIVATE uuid)
endif()

target_include_directories(engineTests PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/lib/glad/include
)

include(GoogleTest)
gtest_discover_tests(engineTests)
