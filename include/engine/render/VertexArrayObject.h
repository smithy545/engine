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

#ifndef ENGINE_VERTEXARRAYOBJECT_H
#define ENGINE_VERTEXARRAYOBJECT_H

#include <engine/render/buffer_objects.h>
#include <engine/render/VertexAttribute.h>
#include <GL/glew.h>
#include <utility>
#include <vector>


namespace engine::render {

class VertexArrayObject {
public:
	USING_PTR(VertexArrayObject);

	virtual ~VertexArrayObject() {
		if (m_id)
			glDeleteVertexArrays(1, &m_id);
	}

	void generate() {
		glGenVertexArrays(1, &m_id);
	}

	void bind() const {
		glBindVertexArray(m_id);
	}

	virtual ElementBuffer::Ptr get_element_buffer() = 0;

	virtual std::vector<std::pair<BufferObject::Ptr, VertexAttribute>> get_attribute_buffers() = 0;

private:
	GLuint m_id{0};
};

} // namespace engine::render

#endif //ENGINE_VERTEXARRAYOBJECT_H
