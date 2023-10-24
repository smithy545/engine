/*
MIT License
Copyright (c) 2022 Philip Arturo Smith
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

#ifndef ENGINE_SHADER_H
#define ENGINE_SHADER_H

#include <engine/render/renderer.h>
#include <GL/glew.h>
#include <iostream>
#include <utils/file_util.h>


namespace engine::render {

class Shader {
public:
	Shader(std::string vertex_path, std::string fragment_path, std::string geometry_path = "") {
		try {
			auto vertex_src = utils::file::read_file_to_string(vertex_path);
			auto fragment_src = utils::file::read_file_to_string(fragment_path);
			if (geometry_path.empty())
				m_id = load_shader(vertex_src.c_str(), fragment_src.c_str());
			else {
				auto geometry_src = utils::file::read_file_to_string(geometry_path);
				m_id = load_shader(vertex_src.c_str(), fragment_src.c_str(), geometry_src.c_str());
			}
		} catch(std::runtime_error e) {
			std::cerr << e.what() << std::endl;
		}
	}

	void use() {
		glUseProgram(m_id);
	}

	void destroy() {
		glDeleteProgram(m_id);
	}

	// REF: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader.h
	// utility uniform functions
	// ------------------------------------------------------------------------
	void uniform_bool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void uniform_int(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void uniform_float(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void uniform_vec2(const std::string &name, const glm::vec2 &value) const
	{
		glUniform2fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
	}
	void uniform_vec2(const std::string &name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(m_id, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void uniform_vec3(const std::string &name, const glm::vec3 &value) const
	{
		glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
	}
	void uniform_vec3(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(m_id, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void uniform_vec4(const std::string &name, const glm::vec4 &value) const
	{
		glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
	}
	void uniform_vec4(const std::string &name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(m_id, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void uniform_mat2(const std::string &name, const glm::mat2 &mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void uniform_mat3(const std::string &name, const glm::mat3 &mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void uniform_mat4(const std::string &name, const glm::mat4 &mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

private:
	GLuint m_id;
};

} // namespace engine::render

#endif //ENGINE_SHADER_H
