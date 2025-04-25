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

#include <chrono>
#include <engine/render/Glyph.h>
#include <engine/render/RenderContext.h>
#include <entt/entt.hpp>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <utils/macros.h>

// essentially a singleton namespace. Private "member" variables/functions are in renderer.cpp anonymous namespace
// Might switch away from this if I have  a good reason
namespace engine::render {

bool init();

void render(std::chrono::nanoseconds dt);

void swap_buffers();

void clear_screen();

void cleanup();

// state access
GLFWwindow *get_window();

entt::registry &get_registry();

const RenderContext &get_context();

glm::mat4 get_projection();

void load_texture(GLuint *texture, unsigned int width, unsigned int height, int internalformat, int format, int type, void *data);

GLuint load_transform_shader(const char *vertex_source, const char *fragment_source, const char *geometry_source);

GLuint load_shader(const char *vertex_source, const char *frag_source, const char *geom_source = nullptr);

std::map<unsigned long, Glyph> load_font(
		FT_Library ft,
		const std::string &fontfile,
		unsigned int font_size,
		const std::string &text =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,?!-:!@#$%^&*()_+|~");

} // namespace engine::render

#endif //ENGINE_RENDERER_H
