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

#include "RenderContext.h"


namespace engine {
    class Renderer {
    public:
        explicit Renderer(RenderContext& context);

        static bool init(entt::registry &registry);

        void render(entt::registry &registry);

        void cleanup(entt::registry &registry);

        static void load_mesh(entt::registry &registry, entt::entity entity);

        static void load_sprite(entt::registry &registry, entt::entity entity);

        static void destroy_vao(entt::registry &registry, entt::entity entity);

        static void destroy_instances(entt::registry &registry, entt::entity entity);
    private:
        const RenderContext& ctx;

        static bool init_glfw();

        static bool init_glew();

        void read_config(RenderContext& context, const std::string& filename);

        bool init_window(RenderContext& context);

        bool init_shaders(RenderContext& context);
    };
} // namespace engine

#endif //ENGINE_RENDERER_H
