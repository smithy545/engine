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

#ifndef ENGINE_VERTEXATTRIBUTE_H
#define ENGINE_VERTEXATTRIBUTE_H

#include <GL/glew.h>

namespace engine::render {

// Attribute info storage class
class VertexAttribute {
public:
	USEPTR(VertexAttribute);

	explicit VertexAttribute(
			GLint num_components,
			GLenum type = GL_FLOAT,
			bool normalized = false,
			GLsizei stride = 0,
			const void* initial_offset = nullptr)
			: m_size(num_components),
			  m_data_type(type),
			  m_normalized(normalized),
			  m_stride(stride),
			  m_offset(initial_offset) {}

	GLint get_num_components() const {
		return m_size;
	}

	GLenum get_data_type() const {
		return m_data_type;
	}

	bool normalized() const {
		return m_normalized;
	}

	GLsizei get_stride() const {
		return m_stride;
	}

	const void* get_pointer_offset() const {
		return m_offset;
	}

	void bind(GLuint index, GLuint divisor = 0) const {
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(
				index,
				m_size,
				m_data_type,
				m_normalized,
				m_stride,
				m_offset);
		glVertexAttribDivisor(index, divisor);
	}
private:
	GLint m_size;
	GLenum m_data_type;
	bool m_normalized;
	GLsizei m_stride;
	const void* m_offset;
};

} // namespace engine::render

#endif //ENGINE_VERTEXATTRIBUTE_H
