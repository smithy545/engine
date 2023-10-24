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

#ifndef ENGINE_INSTANCE_CONTAINERS_H
#define ENGINE_INSTANCE_CONTAINERS_H

#include <engine/render/buffer_objects.h>
#include <engine/render/VertexAttribute.h>
#include <engine/render/glm_attributes.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>


namespace engine::render {

struct InstanceSet {
	USEPTR(InstanceSet);

	virtual std::size_t num_instances() const = 0;

	virtual GLsizeiptr num_indices() const = 0;

	virtual GLuint get_render_strategy() const = 0;

	virtual GLuint get_divisor() const = 0;

	virtual std::vector<VertexAttribute> get_attributes() const = 0;
};

template <typename ElementType>
class InstanceVector : public InstanceSet, public BufferObject {
public:
	USEPTR(InstanceVector);

	InstanceVector(GLuint render_strategy, GLsizeiptr index_count)
			: BufferObject(GL_ARRAY_BUFFER), m_render_strategy(render_strategy), m_num_indices(index_count) {}

	void bind_to_vao(GLuint index_offset = 0) {
		bind();
		buffer();
		auto attrs = get_attributes();
		auto divisor = get_divisor();
		for (const auto &attr: attrs) {
			attr.bind(index_offset, divisor);
			++index_offset;
		}
	}

	GLuint get_divisor() const override {
		return 1;
	}

	std::size_t num_instances() const override {
		return m_transformations.size();
	}

	GLsizeiptr num_indices() const override {
		return m_num_indices;
	}

	GLuint get_render_strategy() const override {
		return m_render_strategy;
	}

	GLsizeiptr get_byte_size() override {
		return m_transformations.size() * sizeof(ElementType);
	}

	const void* get_data() override {
		return &m_transformations[0];
	}

	const std::vector<ElementType>& get_data_vector() {
		return m_transformations;
	}

	void set_data(std::vector<ElementType> data) {
		m_transformations = std::move(data);
	}

	void set_num_indices(GLuint count) {
		m_num_indices = count;
	}

	void set_render_strategy(GLuint render_strategy) {
		m_render_strategy = render_strategy;
	}
private:
	std::vector<ElementType> m_transformations;
	GLuint m_render_strategy;
	GLsizeiptr m_num_indices;
};


struct Mat2Instances : public InstanceVector<glm::mat2> {
	Mat2Instances(GLuint render_strat, GLsizeiptr index_count)
	: InstanceVector<glm::mat2>(render_strat, index_count) {}

	[[nodiscard]] std::vector<VertexAttribute> get_attributes() const override {
		auto sizeof_vec2 = sizeof(glm::vec2);
		auto stride = 2 * sizeof_vec2;
		return {
			Vec2Attribute(GL_FLOAT, false, stride, nullptr),
			Vec2Attribute(GL_FLOAT, false, stride, (void*) sizeof_vec2)
		};
	}
};

struct Mat4Instances : public InstanceVector<glm::mat4> {
	Mat4Instances(GLuint render_strat, GLsizeiptr index_count)
	: InstanceVector<glm::mat4>(render_strat, index_count) {}

	[[nodiscard]] std::vector<VertexAttribute> get_attributes() const override {
		auto sizeof_vec4 = sizeof(glm::vec4);
		auto stride = 4 * sizeof_vec4;
		return {
				Vec4Attribute(GL_FLOAT, false, stride, (void*) 0),
				Vec4Attribute(GL_FLOAT, false, stride, (void*) sizeof_vec4),
				Vec4Attribute(GL_FLOAT, false, stride, (void*) (2 * sizeof_vec4)),
				Vec4Attribute(GL_FLOAT, false, stride, (void*) (3 * sizeof_vec4)),
		};
	}
};


} // namespace engine::render

#endif //ENGINE_INSTANCE_CONTAINERS_H
