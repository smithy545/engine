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

#include <engine/render/buffer_objects.h>
#include <engine/render/VertexArrayObject.h>
#include <engine/render/glm_attributes.h>
#include <glm/glm.hpp>
#include <utility>
#include <vector>


namespace engine::render {

template <typename VertexType = glm::vec3, typename ColorType = glm::vec3, typename TexCoordType = glm::vec2>
class Mesh : public VertexArrayObject {
public:
	USEPTR(Mesh);

	using VertexBufferType = ArrayBuffer<VertexType>;
	using ColorBufferType = ArrayBuffer<ColorType>;
	using TexCoordBufferType = ArrayBuffer<TexCoordType>;

	Mesh() : VertexArrayObject(),
	         m_vert_coords(std::make_shared<VertexBufferType>()),
	         m_colors(std::make_shared<ColorBufferType>()),
	         m_tex_coords(std::make_shared<TexCoordBufferType>()),
	         m_indices{std::make_shared<ElementBuffer>()} {}

	Mesh(VertexBufferType::Ptr vert_buffer, ColorBufferType::Ptr color_buffer, ElementBuffer::Ptr index_buffer)
			: m_vert_coords(std::move(vert_buffer)),
			  m_colors(std::move(color_buffer)),
			  m_tex_coords(std::make_shared<TexCoordBufferType>()),
			  m_indices(std::move(index_buffer)) {}

	Mesh(VertexBufferType::Ptr vert_buffer, TexCoordBufferType::Ptr m_tex_coords, ElementBuffer::Ptr index_buffer)
			: m_vert_coords(std::move(vert_buffer)),
			  m_colors(std::make_shared<ColorBufferType>()),
			  m_tex_coords(std::move(m_tex_coords)),
			  m_indices(std::move(index_buffer)) {}

	std::vector<std::pair<BufferObject::Ptr, VertexAttribute>> get_attribute_buffers() override {
		return {
				std::pair<BufferObject::Ptr, VertexAttribute>{m_vert_coords, Vec3Attribute()},
				std::pair<BufferObject::Ptr, VertexAttribute>{m_colors, Vec3Attribute()},
				std::pair<BufferObject::Ptr, VertexAttribute>{m_tex_coords, Vec2Attribute()}
		};
	}

	ElementBuffer::Ptr get_element_buffer() override {
		return m_indices;
	}

	VertexBufferType::Ptr get_vertex_buffer() {
		return m_vert_coords;
	}

	ColorBufferType::Ptr get_color_buffer() {
		return m_colors;
	}

	TexCoordBufferType::Ptr get_uv_buffer() {
		return m_tex_coords;
	}

	GLuint *get_texture() {
		return &m_texture;
	}

protected:
	VertexBufferType::Ptr m_vert_coords;
	ColorBufferType::Ptr m_colors;
	TexCoordBufferType::Ptr m_tex_coords;
	ElementBuffer::Ptr m_indices;
	GLuint m_texture{0};

private:
	GLuint m_id{0};
};

} // namespace engine::render

#endif //ENGINE_MESH_H
