/* Created by Philip Smith on 5/13/21.
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

#ifndef ENGINE_MESH_H
#define ENGINE_MESH_H

#include <engine/render/BufferObject.h>
#include <engine/render/VertexArrayObject.h>
#include <engine/render/glm_attributes.h>
#include <glm/glm.hpp>
#include <utility>
#include <vector>


namespace engine::render {

class Mesh : public VertexArrayObject {
public:
	Mesh() : VertexArrayObject(),
	m_vertices(std::make_shared<Vec3Buffer>(GL_ARRAY_BUFFER)),
	m_colors(std::make_shared<Vec3Buffer>(GL_ARRAY_BUFFER)),
	m_indices{std::make_shared<ElementBuffer>()} {}

	std::vector<std::pair<BufferObject::Ptr, VertexAttribute>> get_attribute_buffers() override {
		return {
			std::pair<BufferObject::Ptr, VertexAttribute>{m_vertices, Vec3Attribute()},
			std::pair<BufferObject::Ptr, VertexAttribute>{m_colors, Vec3Attribute()}
		};
	}

	ElementBuffer::Ptr get_element_buffer() override {
		return m_indices;
	}

	Vec3Buffer::Ptr get_vertex_buffer() {
		return m_vertices;
	}

	Vec3Buffer::Ptr get_color_buffer() {
		return m_colors;
	}
private:
	GLuint m_id;
	Vec3Buffer::Ptr m_vertices;
	Vec3Buffer::Ptr m_colors;
	ElementBuffer::Ptr m_indices;
};

} // namespace engine::render

#endif //ENGINE_MESH_H
