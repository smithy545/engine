cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
add_library(engine
        src/Renderer.cpp
        src/GameManager.cpp
        src/InterfaceController.cpp
        src/InterfaceView.cpp
        src/BoxMesh.cpp
        src/BoxElement.cpp
        src/OrbitCam.cpp
        src/Steadicam.cpp
        src/ShapeElement.cpp)
target_link_directories(engine PRIVATE
        /opt/homebrew/Cellar/glew/2.2.0_1/lib
        /opt/homebrew/Cellar/glfw/3.3.4/lib)
target_link_libraries(engine utils OpenGL::GL GLEW GLFW EnTT CGAL freetype)
target_include_directories(engine
        PRIVATE
        ../../external/cute_headers
        ../../external/freetype/include
        PUBLIC
        /opt/homebrew/Cellar/glew/2.2.0_1/include
        /opt/homebrew/Cellar/glm/0.9.9.8/include
        /opt/homebrew/Cellar/glfw/3.3.4/include
        include)
