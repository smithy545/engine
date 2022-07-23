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

#include <engine/render/renderer.h>
#include <engine/render/InstanceList.h>
#include <engine/render/mesh/Mesh.h>
#include <engine/render/sprite/ShapeSprite.h>
#include <engine/render/sprite/TextSprite.h>
#include <engine/render/sprite/TextureSprite.h>
#include <fmt/format.h>
#include <glm/glm.hpp>
#include <iostream>
#include <stdexcept>
#include <utils/file_util.h>


using namespace utils::concepts;

namespace engine::renderer {
namespace { // pseudo-member namespace
RenderContext context{};
RequestedResources resources{};
} // anonymous

void render(entt::registry &registry) {
	glm::mat4 vp;
	/*
	// 3D rendering
	if (camera != nullptr) {
		vp = glm::perspective(
				ctx.fovy,
				ctx.screen_width / ctx.screen_height,
				ctx.z_near,
				ctx.z_far) * camera->get_view();
		glUseProgram(ctx.color_shader3d);
		glUniformMatrix4fv(glGetUniformLocation(ctx.color_shader3d, "VP"), 1, GL_FALSE, &vp[0][0]);
		auto view3d = registry.view<Mesh, InstanceList>();
		for (const auto &entity: view3d) {
			auto[mesh, ilist] = view3d.get(entity);
			if (mesh.visible) {
				glBindVertexArray(mesh.vao);
				glDrawElementsInstanced(ilist.render_strategy, mesh.num_indices, GL_UNSIGNED_INT, 0,
				                        ilist.instances.size());
			}
		}
	}

	// 2D rendering (TODO: Add occlusion culling to prevent drawing 3D entities that are covered by 2D elements)
	vp = glm::ortho(0.f, 1.f * ctx.screen_width, 1.f * ctx.screen_height, 0.f);
	glUseProgram(ctx.color_shader2d);
	glUniformMatrix4fv(glGetUniformLocation(ctx.color_shader2d, "VP"), 1, GL_FALSE, &vp[0][0]);
	auto color_view = registry.view<ShapeSprite, InstanceList>();
	for (const auto &entity: color_view) {
		auto[sprite, ilist] = color_view.get(entity);
		if (sprite.visible) {
			glBindVertexArray(sprite.vao);
			glDrawElementsInstanced(ilist.render_strategy, sprite.num_indices, GL_UNSIGNED_INT, 0,
			                        ilist.instances.size());
		}
	}

	// texture rendering
	glUseProgram(ctx.tex_shader2d);
	glUniformMatrix4fv(glGetUniformLocation(ctx.tex_shader2d, "VP"), 1, GL_FALSE, &vp[0][0]);
	glUniform1i(glGetUniformLocation(ctx.tex_shader2d, "texSampler"), 0);
	auto tex_view = registry.view<TextureSprite, InstanceList>();
	for (const auto &entity: tex_view) {
		auto[sprite, ilist] = tex_view.get(entity);
		if (sprite.visible) {
			glBindVertexArray(sprite.vao);
			glBindTexture(GL_TEXTURE_2D, sprite.tex_id);
			glDrawElementsInstanced(ilist.render_strategy, sprite.num_indices, GL_UNSIGNED_INT, 0,
			                        ilist.instances.size());
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindVertexArray(0);
		}
	}

	// text rendering
	glUseProgram(ctx.text_shader);
	glUniformMatrix4fv(glGetUniformLocation(ctx.text_shader, "VP"), 1, GL_FALSE, &vp[0][0]);
	glUniform1i(glGetUniformLocation(ctx.text_shader, "texSampler"), 0);
	auto text_view = registry.view<TextSprite>();
	for (const auto &entity: text_view) {
		auto[sprite] = text_view.get(entity);
		if (sprite.visible) {
			if (!m_loaded_fonts.contains(sprite.font))
				std::cerr << "Can't render sprite with unloaded font '" << sprite.font << "'" << std::endl;
			else {
				glUniform3f(glGetUniformLocation(ctx.text_shader, "textColor"),
				            sprite.color.x, sprite.color.y, sprite.color.z);
				auto x = sprite.x;
				for (std::string::const_iterator c = sprite.text.begin(); c != sprite.text.end(); c++) {
					auto font = m_loaded_fonts.at(sprite.font);
					if (!font.contains(*c)) {
						std::cerr << "Todo: add 404 texture. Error on '" << *c << "'" << std::endl;
						continue;
					}
					auto glyph = font[*c];
					float xpos = x + glyph.bearing.x * sprite.scale;
					float ypos = sprite.y - (glyph.size.y - glyph.bearing.y) * sprite.scale;

					float w = glyph.size.x * sprite.scale;
					float h = glyph.size.y * sprite.scale;
					// update VBO for each character
					float vertices[6][4] = {
							{xpos,     ypos - h, 0.0f, 0.0f},
							{xpos,     ypos,     0.0f, 1.0f},
							{xpos + w, ypos,     1.0f, 1.0f},
							{xpos,     ypos - h, 0.0f, 0.0f},
							{xpos + w, ypos,     1.0f, 1.0f},
							{xpos + w, ypos - h, 1.0f, 0.0f}
					};
					glBindVertexArray(sprite.vao);
					// render glyph texture over quad
					glBindTexture(GL_TEXTURE_2D, glyph.tex_id);
					// update content of VBO memory
					glBindBuffer(GL_ARRAY_BUFFER, sprite.vbo);
					glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					// render quad
					glDrawArrays(GL_TRIANGLES, 0, 6);
					// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
					x += (glyph.advance >> 6) * sprite.scale; // bitshift by 6 to get value in pixels (2^6 = 64)
					glBindTexture(GL_TEXTURE_2D, 0);
					glBindVertexArray(0);
				}
			}
		}
	}
	glUseProgram(0);*/
}

void init(entt::registry &registry) {
	if (!init_context("../res/bootstrap.json"))
		throw std::runtime_error("Failed to read config");
	if (!init_glfw())
		throw std::runtime_error("Failed to init_scene GLFW");
	if (!init_window())
		throw std::runtime_error{"Failed to init_scene window"};
	if (!init_glew())
		throw std::runtime_error{"Failed to init_scene glew"};

	register_entt_callbacks(registry);

	// cull triangles facing away from camera
	glEnable(GL_CULL_FACE);
	// enable depth buffer
	glEnable(GL_DEPTH_TEST);
	// background color0
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

bool init_context(const std::string &config_path) {
	// defaults
	context.screen_width = 800;
	context.screen_height = 600;
	context.fovy = 45.0f;
	context.z_near = 0.1f;
	context.z_far = 10000.0f;
	try {
		// load override from config file if available
		auto config_json = utils::file::read_json_file(config_path);

		if (config_json.contains("width"))
			context.screen_width = config_json["width"];
		if (config_json.contains("height"))
			context.screen_height = config_json["height"];
		if (config_json.contains("fovy"))
			context.fovy = config_json["fovy"];
		if (config_json.contains("z_near"))
			context.z_near = config_json["z_near"];
		if (config_json.contains("z_far"))
			context.z_far = config_json["z_far"];
		if (config_json.contains("resources")) {
			auto resource_json = config_json["resources"];
			// TODO: write json serialization/deserialization code for resource structs
			if (resource_json.contains("textures")) {
				for (auto texture: resource_json["textures"]) {
					if (texture.contains("name") && texture.contains("path")) {
						TextureResource res;
						res.name = texture["name"];
						res.path = texture["path"];
						resources.textures.push_back(res);
					} else
						std::cerr << "Malformed texture resource" << std::endl;
				}
			}
			if (resource_json.contains("fonts")) {
				for (auto font: resource_json["fonts"]) {
					if (font.contains("name") && font.contains("path") && font.contains("size")) {
						FontResource res;
						res.name = font["name"];
						res.path = font["path"];
						res.size = font["size"];
						resources.fonts.push_back(res);
					} else
						std::cerr << "Malformed font resource" << std::endl;
				}
			}
			if (resource_json.contains("shaders")) {
				for (const auto &shader: resource_json["shaders"])
					if (shader.contains("name")
					&& shader.contains("vertex")
					&& shader.contains("fragment")) {
						ShaderResource res;
						res.name = shader["name"];
						res.vertex_path = shader["vertex"];
						res.fragment_path = shader["fragment"];
						resources.shaders.push_back(res);
					} else
						std::cerr << "Malformed shader resource" << std::endl;
			}
		}
	} catch (nlohmann::detail::parse_error &e) {
		std::cerr << "HERE: " << std::endl;
		std::cerr << e.what() << std::endl;
		return false;
	}
	return true;
}

bool init_glfw() {
	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	return true;
}

bool init_glew() {
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		return false;
	}

	return true;
}

bool init_window() {
	// Open a window and create its OpenGL context
	context.window = glfwCreateWindow(
			context.screen_width,
			context.screen_height,
			"Civil War",
			nullptr,
			nullptr);
	if (context.window == nullptr) {
		std::cerr << "Failed to open GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(context.window);

	return true;
}

bool init_fonts(const nlohmann::json& fonts) {
	FT_Library ft;
	if(FT_Init_FreeType(&ft)) {
		std::cerr << "Could not initialize FreeType library" << std::endl;
		return false;
	}

	// enable blending for text transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::unordered_map<std::string, std::map<unsigned long, Glyph>> loaded_fonts;
	for(auto font: fonts) {
		auto glyphs = load_font(ft, font["path"], font["size"]);
		if(glyphs.empty())
			std::cerr << "Could not initialize font from '" << font["path"] << "'" << std::endl;
		else
			loaded_fonts[font["name"]] = glyphs;
	}
	FT_Done_FreeType(ft);

	return true;
}

bool init_shaders() {
	auto color_vshader_src = utils::file::read_file_to_string("../res/shaders/vert/color_vert.glsl");
	auto color_vshader_src2d = utils::file::read_file_to_string("../res/shaders/vert/color_vert2d.glsl");
	auto color_fshader_src = utils::file::read_file_to_string("../res/shaders/frag/color_frag.glsl");
	auto tex_vshader_src = utils::file::read_file_to_string("../res/shaders/vert/tex_vert.glsl");
	auto tex_vshader_src2d = utils::file::read_file_to_string("../res/shaders/vert/tex_vert2d.glsl");
	auto tex_fshader_src = utils::file::read_file_to_string("../res/shaders/frag/tex_frag.glsl");
	auto text_vshader_src = utils::file::read_file_to_string("../res/shaders/vert/text_vert.glsl");
	auto text_fshader_src = utils::file::read_file_to_string("../res/shaders/frag/text_frag.glsl");

	try {
		//context.color_shader2d = load_shader(color_vshader_src2d.c_str(), color_fshader_src.c_str());
	} catch (std::runtime_error& e) {
		std::cerr << "2d color shader failed to init: " << e.what() << std::endl;
		return false;
	}
	try {
		//context.color_shader3d = load_shader(color_vshader_src.c_str(), color_fshader_src.c_str());
	} catch (std::runtime_error& e) {
		std::cerr << "3d color shader failed to init: " << e.what() << std::endl;
		return false;
	}
	try {
		//context.tex_shader2d = load_shader(tex_vshader_src2d.c_str(), tex_fshader_src.c_str());
	} catch (std::runtime_error& e) {
		std::cerr << "2d texture shader failed to init: " << e.what() << std::endl;
		return false;
	}
	try {
		//context.tex_shader3d = load_shader(tex_vshader_src.c_str(), tex_fshader_src.c_str());
	} catch (std::runtime_error& e) {
		std::cerr << "3d texture shader failed to init: " << e.what() << std::endl;
		return false;
	}
	try {
		//context.text_shader = load_shader(text_vshader_src.c_str(), text_fshader_src.c_str());
	} catch (std::runtime_error& e) {
		std::cerr << "Text shader failed to init: " << e.what() << std::endl;
		return false;
	}
	return true;
}

void register_entt_callbacks(entt::registry &registry) {
	registry.on_construct<Mesh>().connect<&construct_mesh>();
	registry.on_construct<ShapeSprite>().connect<&construct_shape_sprite>();
	registry.on_construct<TextureSprite>().connect<&construct_texture_sprite>();
	registry.on_construct<TextSprite>().connect<&construct_text_sprite>();
	registry.on_update<Mesh>().connect<&update_mesh>();
	registry.on_update<ShapeSprite>().connect<&update_shape_sprite>();
	registry.on_update<TextureSprite>().connect<&update_texture_sprite>();
	registry.on_destroy<VertexArrayObject>().connect<&destroy_vao>();
	registry.on_destroy<InstanceList>().connect<&destroy_instances>();
}

void cleanup(entt::registry& registry) {
	/*
	glDeleteProgram(context.color_shader2d);
	glDeleteProgram(context.color_shader3d);
	glDeleteProgram(context.tex_shader2d);
	glDeleteProgram(context.tex_shader3d);
	glDeleteProgram(context.text_shader);
	*/
	glfwTerminate();
}

void update_mesh(entt::registry& registry, entt::entity entity) {
	auto sizeof_vec3 = sizeof(glm::vec3);
	auto &mesh = registry.get<Mesh>(entity);
	glBindVertexArray(mesh.vao);
	// verts
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof_vec3 * mesh.vertices.size(), &mesh.vertices[0], GL_STATIC_DRAW);
	// colors
	glBindBuffer(GL_ARRAY_BUFFER, mesh.cbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof_vec3 * mesh.colors.size(), &mesh.colors[0], GL_STATIC_DRAW);
	// indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.indices.size(), &mesh.indices[0],
	             GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void update_shape_sprite(entt::registry& registry, entt::entity entity) {
	auto sizeof_vec3 = sizeof(glm::vec3);
	auto sizeof_vec2 = sizeof(glm::vec2);
	auto &sprite = registry.get<ShapeSprite>(entity);
	glBindVertexArray(sprite.vao);
	// verts
	glBindBuffer(GL_ARRAY_BUFFER, sprite.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof_vec2 * sprite.vertices.size(), &sprite.vertices[0], GL_STATIC_DRAW);
	// colors
	glBindBuffer(GL_ARRAY_BUFFER, sprite.cbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof_vec3 * sprite.colors.size(), &sprite.colors[0], GL_STATIC_DRAW);
	// indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * sprite.indices.size(), &sprite.indices[0],
	             GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void update_texture_sprite(entt::registry& registry, entt::entity entity) {
	auto sizeof_vec2 = sizeof(glm::vec2);
	auto &sprite = registry.get<TextureSprite>(entity);
	glBindVertexArray(sprite.vao);
	// verts
	glBindBuffer(GL_ARRAY_BUFFER, sprite.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof_vec2 * sprite.vertices.size(), &sprite.vertices[0], GL_STATIC_DRAW);
	// uvs
	glBindBuffer(GL_ARRAY_BUFFER, sprite.tex_uvs);
	glBufferData(GL_ARRAY_BUFFER, sizeof_vec2 * sprite.uvs.size(), &sprite.uvs[0], GL_STATIC_DRAW);
	// indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * sprite.indices.size(), &sprite.indices[0],
	             GL_STATIC_DRAW);
	// texture sampler
	//sprite.tex_id = m_loaded_textures[sprite.name];
	glBindVertexArray(0);
}

void destroy_vao(entt::registry& registry, entt::entity entity) {
	auto vao = registry.get<VertexArrayObject>(entity);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	if (vao.vao)
		glDeleteVertexArrays(1, &vao.vao);
	if (vao.vbo)
		glDeleteBuffers(1, &vao.vbo);
	if (vao.ebo)
		glDeleteBuffers(1, &vao.ebo);
	if (vao.cbo)
		glDeleteBuffers(1, &vao.cbo);
	if (vao.tex_id)
		glDeleteTextures(1, &vao.tex_id);
	if (vao.tex_uvs)
		glDeleteBuffers(1, &vao.tex_uvs);
}

void destroy_instances(entt::registry& registry, entt::entity entity) {
	auto instances = registry.get<InstanceList>(entity);
	if (instances.id)
		glDeleteBuffers(1, &instances.id);
}

void construct_mesh(entt::registry& registry, entt::entity entity) {
	auto sizeof_vec4 = sizeof(glm::vec4);
	auto sizeof_vec3 = sizeof(glm::vec3);
	auto &mesh = registry.get<Mesh>(entity);
	// setup vertex array object
	glGenVertexArrays(1, &mesh.vao);
	glBindVertexArray(mesh.vao);

	// verts
	glGenBuffers(1, &mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof_vec3 * mesh.vertices.size(), &mesh.vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// colors
	glGenBuffers(1, &mesh.cbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.cbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof_vec3 * mesh.colors.size(), &mesh.colors[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// indices
	glGenBuffers(1, &mesh.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.indices.size(), &mesh.indices[0],
	             GL_STATIC_DRAW);

	// instance buffer (initially empty)
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ARRAY_BUFFER, ibo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof_vec4, (void *) 0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof_vec4, (void *) (1 * sizeof_vec4));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof_vec4, (void *) (2 * sizeof_vec4));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof_vec4, (void *) (3 * sizeof_vec4));
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	mesh.num_indices = mesh.indices.size();
	registry.emplace<InstanceList>(entity, ibo);
	glBindVertexArray(0);
}

void construct_shape_sprite(entt::registry& registry, entt::entity entity) {
	auto sizeof_vec4 = sizeof(glm::vec4);
	auto &sprite = registry.get<ShapeSprite>(entity);
	// setup vertex array object
	glGenVertexArrays(1, &sprite.vao);
	glBindVertexArray(sprite.vao);

	// verts
	glGenBuffers(1, &sprite.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, sprite.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * sprite.vertices.size(), &sprite.vertices[0],
	             GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// colors
	glGenBuffers(1, &sprite.cbo);
	glBindBuffer(GL_ARRAY_BUFFER, sprite.cbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * sprite.colors.size(), &sprite.colors[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// indices
	glGenBuffers(1, &sprite.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * sprite.indices.size(), &sprite.indices[0],
	             GL_STATIC_DRAW);

	// instance buffer (initially empty)
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ARRAY_BUFFER, ibo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof_vec4, (void *) 0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof_vec4, (void *) (1 * sizeof_vec4));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof_vec4, (void *) (2 * sizeof_vec4));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof_vec4, (void *) (3 * sizeof_vec4));
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glBindVertexArray(0);
	sprite.num_indices = sprite.indices.size();
	registry.emplace<InstanceList>(entity, ibo);
}

void construct_text_sprite(entt::registry& registry, entt::entity entity) {
	auto &sprite = registry.get<TextSprite>(entity);
	glGenVertexArrays(1, &sprite.vao);
	glGenBuffers(1, &sprite.vbo);
	glBindVertexArray(sprite.vao);
	glBindBuffer(GL_ARRAY_BUFFER, sprite.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void construct_texture_sprite(entt::registry& registry, entt::entity entity) {
	auto sizeof_vec2 = sizeof(glm::vec2);
	auto sizeof_vec4 = sizeof(glm::vec4);
	auto &sprite = registry.get<TextureSprite>(entity);
	// setup vertex array object
	glGenVertexArrays(1, &sprite.vao);
	glBindVertexArray(sprite.vao);

	// verts
	glGenBuffers(1, &sprite.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, sprite.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof_vec2 * sprite.vertices.size(), &sprite.vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// uvs
	glGenBuffers(1, &sprite.tex_uvs);
	glBindBuffer(GL_ARRAY_BUFFER, sprite.tex_uvs);
	glBufferData(GL_ARRAY_BUFFER, sizeof_vec2 * sprite.uvs.size(), &sprite.uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// indices
	glGenBuffers(1, &sprite.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * sprite.indices.size(), &sprite.indices[0],
	             GL_STATIC_DRAW);

	// texture sampler
	//sprite.texture = m_loaded_textures[sprite.name];

	// instance buffer (initially empty)
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ARRAY_BUFFER, ibo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof_vec4, (void *) 0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof_vec4, (void *) (1 * sizeof_vec4));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof_vec4, (void *) (2 * sizeof_vec4));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof_vec4, (void *) (3 * sizeof_vec4));
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glBindVertexArray(0);
	sprite.num_indices = sprite.indices.size();
	registry.emplace<InstanceList>(entity, ibo);
}

GLuint load_texture(const std::string& path) {
	// TODO
	return 0;
}

GLuint load_shader(const char *vertex_source, const char *frag_source) {
	int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
	glCompileShader(vertex_shader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, nullptr, infoLog);
		std::string message = fmt::format("Error on vertex compilation ", infoLog);
		throw std::runtime_error(message.c_str());
	}
	int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &frag_source, nullptr);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, nullptr, infoLog);
		std::string message = fmt::format("Error on fragment compilation ", infoLog);
		throw std::runtime_error(message.c_str());
	}
	auto shader = glCreateProgram();
	glAttachShader(shader, vertex_shader);
	glAttachShader(shader, fragment_shader);
	glLinkProgram(shader);
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader, 512, nullptr, infoLog);
		std::string message = fmt::format("Error linking program ", infoLog);
		throw std::runtime_error(message.c_str());
	}
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	return shader;
}

std::map<unsigned long, Glyph> load_font(FT_Library ft,
										const std::string& fontfile,
										unsigned int font_size,
										const std::string& text) {
	std::cout << "Loading font at '" << fontfile << "'" << std::endl;
	FT_Face face;
	if (FT_New_Face(ft, fontfile.c_str(), 0, &face)) {
		std::cerr << "Could not initialize font from file at " << fontfile << std::endl;
		FT_Done_Face(face);
		return {};
	}
	// set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, font_size);

	// disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	std::map<unsigned long, Glyph> glyphs;
	for (unsigned char c: text) {
		if (glyphs.contains(c))
			continue;
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph '" << c << "'" << std::endl;
			continue;
			}
		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RED,
		face->glyph->bitmap.width,
		face->glyph->bitmap.rows,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		face->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Glyph glyph = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<unsigned int>(face->glyph->advance.x)
		};
		glyphs.insert({c, glyph});
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	return glyphs;
}

const RenderContext& get_context() {
	return context;
}

std::map<unsigned long, Glyph> get_font(const string& name) {
	return {};
}

GLuint get_shader(const string& name) {
	return 0;
}

GLuint get_texture(const string& name) {
	return 0;
}
} // namespace engine::renderer
