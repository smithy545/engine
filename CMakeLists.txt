cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
add_library(engine
        src/BoxMesh.cpp
        src/EntityMap.cpp
        src/OrbitCam.cpp
        src/renderer.cpp
        src/state.cpp
        src/Steadicam.cpp
        src/interface.cpp
        src/execution.cpp)
target_link_directories(engine
        PRIVATE
        /opt/homebrew/Cellar/glew/2.2.0_1/lib
        /opt/homebrew/Cellar/glfw/3.3.8/lib)
target_link_libraries(engine utils OpenGL::GL GLEW GLFW freetype)
target_include_directories(engine
        PRIVATE
        /opt/homebrew/Cellar/freetype/2.12.1/include/freetype2
        /opt/homebrew/Cellar/cpp-gsl/4.0.0/include
        /opt/homebrew/Cellar/glew/2.2.0_1/include
        /opt/homebrew/Cellar/glm/0.9.9.8/include
        /opt/homebrew/Cellar/glfw/3.3.8/include
        PUBLIC
        include)
