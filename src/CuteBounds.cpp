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
#include <engine/interface/CuteBounds.h>


namespace engine::interface {

CuteBounds::CuteBounds(c2x* transform) : m_type(C2_TYPE_NONE), m_transform(transform) {}

CuteBounds::CuteBounds(c2Circle circle, c2x* transform) : m_type (C2_TYPE_CIRCLE), m_transform(transform) {
	m_structure.circle = circle;
}

CuteBounds::CuteBounds(c2AABB box, c2x* transform) : m_type (C2_TYPE_AABB), m_transform(transform) {
	m_structure.aabb = box;
}

CuteBounds::CuteBounds(c2Capsule capsule, c2x* transform) : m_type (C2_TYPE_CAPSULE), m_transform(transform) {
	m_structure.capsule = capsule;
}

CuteBounds::CuteBounds(c2Poly polygon, c2x* transform) : m_type (C2_TYPE_POLY), m_transform(transform) {
	m_structure.polygon = polygon;
}

bool CuteBounds::collides(float x, float y) {
	c2Circle point{
		c2v{x, y},
		0.f
	};
	return c2Collided(&m_structure, m_transform, m_type, &point, nullptr, C2_TYPE_CIRCLE);
}

bool CuteBounds::collides(CuteBounds other) {
	return c2Collided(&m_structure, m_transform, m_type, &other.m_structure, other.m_transform, other.m_type);
}

c2x CuteBounds::get_transform() {
	if(m_transform == nullptr)
		return c2xIdentity();
	return *m_transform;
}

} // namespace engine::interface