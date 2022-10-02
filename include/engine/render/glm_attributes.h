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

#ifndef ENGINE_GLM_STORAGE_H
#define ENGINE_GLM_STORAGE_H

#include <engine/render/BufferObject.h>
#include <engine/render/VertexAttribute.h>
#include <glm/glm.hpp>


namespace engine::render {

using Vec3Buffer = VectoredBufferObject<glm::vec3>;

struct Vec2Attribute : public VertexAttribute {
	explicit Vec2Attribute(GLenum type = GL_FLOAT,
	                       bool normalized = false,
	                       GLsizei stride = 0,
	                       const void* initial_offset = nullptr)
						   : VertexAttribute(2, type, normalized, stride, initial_offset) {}
};

struct Vec3Attribute : public VertexAttribute {
	explicit Vec3Attribute(GLenum type = GL_FLOAT,
				  bool normalized = false,
				  GLsizei stride = 0,
				  const void* initial_offset = nullptr)
				  : VertexAttribute(3, type, normalized, stride, initial_offset) {}
};

struct Vec4Attribute : public VertexAttribute {
	explicit Vec4Attribute(GLenum type = GL_FLOAT,
				  bool normalized = false,
				  GLsizei stride = 0,
				  const void* initial_offset = nullptr)
				  : VertexAttribute(4, type, normalized, stride, initial_offset) {}
};

} // namespace engine::render

#endif //ENGINE_GLM_STORAGE_H
