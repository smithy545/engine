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

#include <engine/render/buffer_objects.h>
#include <engine/render/camera/Camera.h>
#include <engine/render/glm_attributes.h>
#include <engine/render/instance_containers.h>
#include <engine/render/Mesh.h>
#include <engine/render/Shader.h>
#include <fmt/format.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <stdexcept>
#include <utils/file_util.h>


namespace engine::render {

namespace { // pseudo-member namespace

entt::registry s_registry;
entt::entity s_window_entity;

void print_glfw_error(const char* text) {
	const char** description;
	glfwGetError(description);
	std::cerr << text << std::endl << *description << std::endl;
}

bool init_glfw() {
	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
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
	s_window_entity = s_registry.create();
	auto& window = s_registry.emplace<GLFWwindow*>(s_window_entity);
	auto& context = s_registry.emplace<RenderContext>(s_window_entity);
	context.screen_width = 800;
	context.screen_height = 600;
	context.fovy = 45.0f;
	context.z_near = 0.1f;
	context.z_far = 1000.0f;
	// Open a window and create its OpenGL context
	window = glfwCreateWindow(
			context.screen_width,
			context.screen_height,
			"My Window",
			nullptr,
			nullptr);
	if (window == nullptr) {
		print_glfw_error("Failed to open GLFW window");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window); // focus

	// set window projection matrix
	s_registry.emplace<glm::mat4>(
			s_window_entity,
			glm::ortho(0.f, 1.f * context.screen_width, 1.f * context.screen_height, 0.f));

	return true;
}

// entt object lifecycles
void construct_mesh(entt::registry& registry, entt::entity entity) {
	auto& mesh = registry.get<Mesh<>>(entity);
	mesh.generate();
	mesh.bind();
	auto index = 0;
	auto attribute_data = mesh.get_attribute_buffers();
	for(const auto& pair: attribute_data) {
		pair.first->generate();
		pair.first->bind();
		pair.first->buffer();
		pair.second.bind(index); // divisor 0
		++index;
	}
	mesh.get_element_buffer()->generate();
	mesh.get_element_buffer()->bind();
	mesh.get_element_buffer()->buffer();
	auto& instances = s_registry.emplace<Mat4Instances>(entity, GL_TRIANGLES, mesh.get_element_buffer()->count());
	instances.generate();
	instances.bind_to_vao(index);
	glBindVertexArray(0);
}

void update_mesh(entt::registry& registry, entt::entity entity) {
	auto& mesh = registry.get<Mesh<>>(entity);
	mesh.bind();
	auto attribute_data = mesh.get_attribute_buffers();
	for(const auto& pair: attribute_data) {
		pair.first->bind();
		pair.first->buffer();
	}
	mesh.get_element_buffer()->bind();
	mesh.get_element_buffer()->buffer();
	s_registry.patch<Mat4Instances>(entity, [&](Mat4Instances &instances) {
		instances.set_num_indices(mesh.get_element_buffer()->count());
	});
	glBindVertexArray(0);
}

void update_mat4_instances(entt::registry& registry, entt::entity entity) {
	auto& instances = registry.get<Mat4Instances>(entity);
	auto& mesh = registry.get<Mesh<>>(entity);
	mesh.bind();
	instances.bind();
	instances.buffer();
	glBindVertexArray(0);
}

void register_entt_callbacks() {
	s_registry.on_construct<Mesh<>>().connect<&construct_mesh>();
	s_registry.on_update<Mesh<>>().connect<&update_mesh>();
	s_registry.on_update<Mat4Instances>().connect<&update_mat4_instances>();
}

} // anonymous

bool init() {
	if (!init_glfw()) {
		std::cerr << "Failed to init GLFW" << std::endl;
		return false;
	}
	if (!init_window()) {
		std::cerr << "Failed to init window" << std::endl;
		return false;
	}
	if (!init_glew()) {
		std::cerr << "Failed to init GLEW" << std::endl;
		return false;
	}

	register_entt_callbacks();

	// cull triangles facing away from camera
	glEnable(GL_CULL_FACE);
	// enable depth buffer
	glEnable(GL_DEPTH_TEST);
	// background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	return true;
}

void render_scene(std::chrono::nanoseconds dt, entt::entity scene_entity) {
	// interface/foreground render
	/*auto projection = s_registry.get<glm::mat4>(s_window_entity);
	auto shader = s_registry.get<Shader>(s_window_entity);
	shader.use();
	shader.uniform_mat4("projection", projection);
	auto view = s_registry.view<Mat2Instances>();
	for(auto e: view) {
		auto& instances = view.get<Mat2Instances>(e);
		instances.bind();
		glDrawElementsInstanced(instances.get_render_strategy(),
		                        instances.num_indices(),
		                        GL_UNSIGNED_INT,
		                        nullptr,
		                        instances.num_instances());
	}*/
}

void render(std::chrono::nanoseconds dt) {
	auto cameras = s_registry.view<Camera::Ptr>();
	for(auto entity: cameras) {
		auto camera = s_registry.get<Camera::Ptr>(entity);
		auto shader = s_registry.get<Shader>(entity);
		shader.use();
		shader.uniform_mat4("vp", get_projection() * camera->get_view());

		// TODO: rerender only relevant matrices (i.e. update, new, cull old matrices etc.)
		auto view3d = s_registry.view<Mat4Instances>();
		for (auto e: view3d) {
			auto &instances = view3d.get<Mat4Instances>(e);
			auto &mesh = s_registry.get<Mesh<>>(e);
			auto texture = *mesh.get_texture();
			if (texture) {
				shader.uniform_int("tex0", texture); // setup texture0
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture);
			}
			mesh.bind();
			glDrawElementsInstanced(instances.get_render_strategy(),
									instances.num_indices(),
									GL_UNSIGNED_INT,
									(void*) 0,
									instances.num_instances());
			glBindVertexArray(0);
		}
	}
}

void swap_buffers() {
	auto window = s_registry.get<GLFWwindow*>(s_window_entity);
	glfwSwapBuffers(window);
}

void clear_screen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void cleanup() {
	auto view = s_registry.view<Shader>();
	for(auto e: view)
		view.get<Shader>(e).destroy();
	glfwTerminate();
	s_registry.clear();
}

void load_texture(GLuint *texture, unsigned int width, unsigned int height, int internalformat, int format, int type,
                  void *data) {
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

GLuint load_transform_shader(const char *vertex_source, const char *fragment_source, const char *geometry_source) {
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
	glShaderSource(fragment_shader, 1, &fragment_source, nullptr);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, nullptr, infoLog);
		std::string message = fmt::format("Error on fragment compilation ", infoLog);
		throw std::runtime_error(message.c_str());
	}
	// load geometry shader if given
	int geometry_shader;
	if(geometry_source != nullptr) {
		geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry_shader, 1, &geometry_source, nullptr);
		glCompileShader(geometry_shader);
		glGetShaderiv(geometry_shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(geometry_shader, 512, nullptr, infoLog);
			std::string message = fmt::format("Error on geometry compilation ", infoLog);
			throw std::runtime_error(message.c_str());
		}
	}
	auto shader = glCreateProgram();
	glAttachShader(shader, vertex_shader);
	glAttachShader(shader, fragment_shader);
	if(geometry_source != nullptr)
		glAttachShader(shader, geometry_shader);
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
	if(geometry_source != nullptr)
		glDeleteShader(geometry_shader);
	return shader;
}

GLuint load_shader(const char *vertex_source, const char *fragment_source, const char *geometry_source) {
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
	glShaderSource(fragment_shader, 1, &fragment_source, nullptr);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, nullptr, infoLog);
		std::string message = fmt::format("Error on fragment compilation ", infoLog);
		throw std::runtime_error(message.c_str());
	}
	// load geometry shader if given
	int geometry_shader;
	if(geometry_source != nullptr) {
		geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry_shader, 1, &geometry_source, nullptr);
		glCompileShader(geometry_shader);
		glGetShaderiv(geometry_shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(geometry_shader, 512, nullptr, infoLog);
			std::string message = fmt::format("Error on geometry compilation ", infoLog);
			throw std::runtime_error(message.c_str());
		}
	}
	auto shader = glCreateProgram();
	glAttachShader(shader, vertex_shader);
	glAttachShader(shader, fragment_shader);
	if(geometry_source != nullptr)
		glAttachShader(shader, geometry_shader);
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
	if(geometry_source != nullptr)
		glDeleteShader(geometry_shader);
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

entt::registry &get_registry() {
	return s_registry;
}

GLFWwindow *get_window() {
	return s_registry.get<GLFWwindow *>(s_window_entity);
}

const RenderContext &get_context() {
	return s_registry.get<RenderContext>(s_window_entity);
}

glm::mat4 get_projection() {
	const auto& context = get_context();
    return glm::perspective(glm::radians(context.fovy),(float)context.screen_width / (float)context.screen_height,context.z_near,context.z_far);
}

} // namespace engine::render
