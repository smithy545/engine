cmake_minimum_required(VERSION 3.16)

find_package(OpenGL REQUIRED)
add_subdirectory(external/glfw)
add_library(engine
        src/Renderer.cpp
        src/State.cpp)
target_link_libraries(engine utils opengl32 glfw glew32)
target_include_directories(engine
        PRIVATE external/glfw/include
        PUBLIC include)
