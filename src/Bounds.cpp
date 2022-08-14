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

#define CUTE_C2_IMPLEMENTATION
#include <engine/interface/Bounds.h>



namespace engine::interface {

Bounds::Bounds(c2x* transform) : m_type(C2_TYPE_NONE), m_transform(transform) {}

Bounds::Bounds(c2Circle circle, c2x* transform) : m_type (C2_TYPE_CIRCLE), m_transform(transform) {
	m_structure.circle = circle;
}

Bounds::Bounds(c2AABB box, c2x* transform) : m_type (C2_TYPE_AABB), m_transform(transform) {
	m_structure.aabb = box;
}

Bounds::Bounds(c2Capsule capsule, c2x* transform) : m_type (C2_TYPE_CAPSULE), m_transform(transform) {
	m_structure.capsule = capsule;
}

Bounds::Bounds(c2Poly polygon, c2x* transform) : m_type (C2_TYPE_POLY), m_transform(transform) {
	m_structure.polygon = polygon;
}

bool Bounds::collides(float x, float y) {
	c2Circle point{
		c2v{x, y},
		0.f
	};
	return c2Collided(&m_structure, m_transform, m_type, &point, nullptr, C2_TYPE_CIRCLE);
}

bool Bounds::collides(Bounds other) {
	return c2Collided(&m_structure, m_transform, m_type, &other.m_structure, other.m_transform, other.m_type);
}

utils::math::Point_2 Bounds::get_position() {
	if(m_transform == nullptr)
		return utils::math::Point_2(0,0);
	return {m_transform->p.x, m_transform->p.y};
}

} // namespace engine::interface