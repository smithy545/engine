//
// Created by Philip Smith on 10/17/2020.
//

#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include <engine/Camera.h>
#include <entt/entt.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <nlohmann/json.hpp>
#include <string>
#include <utils/macros.h>

#include "RenderContext.h"
#include "TextureRGB.h"
#include "sprite/TextSprite.h"


namespace engine {
    class Renderer {
    public:
        explicit Renderer(entt::registry &registry);

        bool init(entt::registry &registry);

        void render(entt::registry &registry, const Camera::Ptr& camera);

        void cleanup(entt::registry &registry);

        const RenderContext& get_context(entt::registry &registry) const;

		void load_rgb_texture(entt::registry &registry, entt::entity entity);

        static void load_mesh(entt::registry &registry, entt::entity entity);

        static void load_shape_sprite(entt::registry &registry, entt::entity entity);

        void load_texture_sprite(entt::registry &registry, entt::entity entity);

        static void load_text_sprite(entt::registry &registry, entt::entity entity);

        static void update_mesh(entt::registry &registry, entt::entity entity);

        static void update_shape_sprite(entt::registry &registry, entt::entity entity);

        void update_texture_sprite(entt::registry &registry, entt::entity entity);

        static void update_text_sprite(entt::registry &registry, entt::entity entity);

        static void destroy_vao(entt::registry &registry, entt::entity entity);

        static void destroy_instances(entt::registry &registry, entt::entity entity);

    private:
    	struct Glyph {
    		unsigned int tex_id;
    		glm::ivec2 size;
    		glm::ivec2 bearing;
    		long advance;
    	};

        entt::entity m_context_entity;
        std::map<std::string, unsigned int> m_loaded_textures;
        std::map<std::string, std::map<unsigned long, Glyph>> m_loaded_fonts;

        bool init_fonts(const nlohmann::json& fonts);

        static bool init_glfw();

        static bool init_glew();

        static bool init_window(RenderContext &context);

        static bool init_shaders(RenderContext &context);

        static GLuint load_shader(const char* vertex_source, const char* frag_source);

        static std::map<unsigned long, Glyph> load_font(FT_Library ft, const std::string& fontfile, unsigned int font_size,
														const std::string& text = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,?!-:!@#$%^&*()_+|~");
    };


} // namespace engine

#endif //ENGINE_RENDERER_H
