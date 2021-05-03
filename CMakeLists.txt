cmake_minimum_required(VERSION 3.16)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
add_library(engine
        src/Renderer.cpp
        src/WindowState.cpp)
target_link_libraries(engine utils OpenGL::GL ${GLEW_LIBRARIES} glfw)
target_include_directories(engine
        PRIVATE ${GLEW_INCLUDE_DIRS}
        PUBLIC include)
