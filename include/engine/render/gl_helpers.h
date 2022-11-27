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

#ifndef ENGINE_GL_HELPERS_H
#define ENGINE_GL_HELPERS_H

#include <engine/render/instance_containers.h>
#include <engine/render/VertexAttribute.h>
#include <gsl/gsl>


namespace engine::render {

void bind_gl_attribute(const VertexAttribute &info, GLuint index, GLuint divisor = 0) {
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(
			index,
			info.get_num_components(),
			info.get_data_type(),
			info.normalized(),
			info.get_stride(),
			info.get_pointer_offset());
	glVertexAttribDivisor(index, divisor);
}

void bind_instance_set(gsl::not_null<InstanceSet *> instances, GLuint index_offset = 0) {
	auto index = index_offset;
	auto attrs = instances->get_attributes();
	auto divisor = instances->get_divisor();
	for (const auto &attr: attrs) {
		bind_gl_attribute(attr, index, divisor);
		++index;
	}
}

} // namespace engine::render

#endif //ENGINE_GL_HELPERS_H
