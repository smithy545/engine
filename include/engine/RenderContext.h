//
// Created by Philip Smith on 6/14/21.
//

#ifndef CIVILWAR_RENDERCONTEXT_H
#define CIVILWAR_RENDERCONTEXT_H

#include <entt/entt.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


namespace engine {
    struct RenderContext {
        GLuint shader2d;
        GLuint shader3d;
        float screen_width;
        float screen_height;
        GLFWwindow* window;
    };
} // namespace engine


#endif //CIVILWAR_RENDERCONTEXT_H
