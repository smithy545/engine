//
// Created by Philip Smith on 6/14/21.
//

#ifndef ENGINE_RENDERCONTEXT_H
#define ENGINE_RENDERCONTEXT_H

#include <engine/Camera.h>
#include <entt/entt.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


namespace engine {
    struct RenderContext {
        Camera::Ptr camera;
        GLuint shader2d;
        GLuint shader3d;
        float screen_width;
        float screen_height;
        float fovy;
        float z_near;
        float z_far;
        GLFWwindow* window;
    };
} // namespace engine


#endif //ENGINE_RENDERCONTEXT_H
