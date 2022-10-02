/* Created by Philip Smith on 5/18/21.
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

#ifndef ENGINE_OPENGL_STORAGE_H
#define ENGINE_OPENGL_STORAGE_H

#include <GL/glew.h>
#include <utils/macros.h>
#include <vector>
#include <iostream>

namespace engine::render {

class BufferObject {
public:
	USING_PTR(BufferObject);

	explicit BufferObject(GLenum target) : m_target(target) {}

	virtual ~BufferObject() {
		if (m_id)
			glDeleteBuffers(1, &m_id);
	}

	void generate() {
		glGenBuffers(1, &m_id);
	}

	void bind() const {
		glBindBuffer(m_target, m_id);
	}

	void buffer() {
		glBufferData(m_target, get_byte_size(), get_data(), m_usage);
	}

	virtual GLuint get_id() const {
		return m_id;
	}

	virtual GLenum get_target() const {
		return m_target;
	}

	virtual GLenum get_usage() const {
		return m_usage;
	}

	virtual GLsizeiptr get_byte_size() = 0;

	virtual const void* get_data() = 0;

private:
	GLuint m_id{0};
	GLenum m_target;
	GLenum m_usage{GL_STATIC_DRAW};
};

template <typename DataType>
class VectoredBufferObject : public BufferObject {
public:
	USING_PTR(VectoredBufferObject);

	explicit VectoredBufferObject(GLenum target) : BufferObject(target) {}

	GLsizeiptr get_byte_size() override {
		return m_data.size() * sizeof(DataType);
	}

	const void* get_data() override {
		return &m_data[0];
	}

	void set_data(std::vector<DataType> new_data) {
		m_data = std::move(new_data);
	}

protected:
	std::vector<DataType> m_data;
};

// Buffer data to be used as indices
struct ElementBuffer : public VectoredBufferObject<unsigned int> {
	USING_PTR(ElementBuffer);

	ElementBuffer() : VectoredBufferObject<unsigned int>(GL_ELEMENT_ARRAY_BUFFER) {}

	GLuint count() {
		return this->get_byte_size() / sizeof(unsigned int);
	}
};

} // namespace engine::render

#endif //ENGINE_OPENGL_STORAGE_H
