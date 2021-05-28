//
// Created by Philip Smith on 10/17/2020.
//

#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include <entt/entt.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <utils/macros.h>


namespace engine {
    class Renderer {
    public:
        bool init(entt::registry &registry);

        void render(entt::registry &registry);

        void resize(float width, float height);

        void cleanup(entt::registry &registry);

        static void load_mesh(entt::registry &registry, entt::entity entity);

        static void load_sprite(entt::registry &registry, entt::entity entity);

        static void destroy_vao(entt::registry &registry, entt::entity entity);

        static void destroy_instances(entt::registry &registry, entt::entity entity);
    private:
        GLuint shader2d;
        GLuint shader3d;

        void read_config(const std::string& filename);

        static bool init_glfw();

        static bool init_glew();

        bool init_window(int width, int height);

        bool init_shaders();

    VAR_GET(entt::entity, camera_entity, public){entt::null};
    VAR_GET(float, width, public){0};
    VAR_GET(float, height, public){0};
    VAR_GET(GLFWwindow*, window, public){nullptr};
    };
} // namespace engine

#endif //ENGINE_RENDERER_H
