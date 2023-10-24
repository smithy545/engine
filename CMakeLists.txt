cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
add_library(engine
        src/EntityMap.cpp
        src/OrbitCam.cpp
        src/renderer.cpp
        src/state.cpp
        src/Steadicam.cpp
        src/interface.cpp
        src/execution.cpp
        src/Widget.cpp
        src/CuteBounds.cpp)
target_link_libraries(engine freetype glfw glew fmt OpenGL::GL)
target_link_directories(engine
        PUBLIC
        /opt/homebrew/Cellar/boost/1.82.0_1/lib
        /opt/homebrew/Cellar/cgal/5.6/lib
        /opt/homebrew/Cellar/entt/3.12.2/lib
        /opt/homebrew/Cellar/fmt/10.1.1/lib
        /opt/homebrew/Cellar/freetype/2.13.2/lib
        /opt/homebrew/Cellar/glew/2.2.0_1/lib
        /opt/homebrew/Cellar/glfw/3.3.8/lib)
target_include_directories(engine
        PUBLIC
        ../../external/cute_headers
        ../../external/utils/include
        /opt/homebrew/Cellar/boost/1.82.0_1/include
        /opt/homebrew/Cellar/cgal/5.6/include
        /opt/homebrew/Cellar/cpp-gsl/4.0.0_1/include
        /opt/homebrew/Cellar/entt/3.12.2/include
        /opt/homebrew/Cellar/fmt/10.1.1/include
        /opt/homebrew/Cellar/freetype/2.13.2/include/freetype2
        /opt/homebrew/Cellar/glew/2.2.0_1/include
        /opt/homebrew/Cellar/glm/0.9.9.8/include
        /opt/homebrew/Cellar/glfw/3.3.8/include
        /opt/homebrew/Cellar/nlohmann-json/3.11.2/include
        include)
