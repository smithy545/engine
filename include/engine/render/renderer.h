/* Created by Philip Smith on 10/17/2020.
MIT License

Copyright (c) 2021 Philip Arturo Smith

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include <concepts>
#include <engine/render/Glyph.h>
#include <engine/render/RenderContext.h>
#include <engine/render/RenderResources.h>
#include <entt/entt.hpp>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <nlohmann/json.hpp>
#include <string>
#include <utils/macros.h>


namespace engine::render {

void render(entt::registry& registry);

bool init(entt::registry &registry);

bool init_context(const std::string &config_path);

bool init_window();

bool init_glfw();

bool init_glew();

bool init_resources();

void register_entt_callbacks(entt::registry &registry);

void cleanup(entt::registry &registry);

// state access
const RenderContext &get_context();

// entt object lifecycles
void construct_mesh(entt::registry& registry, entt::entity entity);

void construct_shape_sprite(entt::registry& registry, entt::entity entity);

void construct_text_sprite(entt::registry& registry, entt::entity entity);

void construct_texture_sprite(entt::registry& registry, entt::entity entity);

void update_texture_sprite(entt::registry& registry, entt::entity entity);

void update_mesh(entt::registry& registry, entt::entity entity);

void update_shape_sprite(entt::registry& registry, entt::entity entity);

void destroy_vao(entt::registry& registry, entt::entity entity);

void destroy_instances(entt::registry& registry, entt::entity entity);

// external resource management
GLuint load_texture(const std::string& path);

GLuint load_shader(const char* vertex_source, const char* frag_source);

std::map<unsigned long, Glyph> load_font(
		FT_Library ft,
		const std::string& fontfile,
		unsigned int font_size,
		const std::string& text =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,?!-:!@#$%^&*()_+|~");
} // namespace engine::render

#endif //ENGINE_RENDERER_H
