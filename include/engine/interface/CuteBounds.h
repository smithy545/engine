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
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EBoundsPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef ENGINE_CUTE_BOUNDS_H
#define ENGINE_CUTE_BOUNDS_H

#include <cute_c2.h>
#include <utils/math_util.h>


namespace engine::interface {


class CuteBounds {
public:
	explicit CuteBounds(c2x* transform = nullptr);

	explicit CuteBounds(c2Circle circle, c2x* transform = nullptr);

	explicit CuteBounds(c2AABB box, c2x* transform = nullptr);

	explicit CuteBounds(c2Capsule capsule, c2x* transform = nullptr);

	explicit CuteBounds(c2Poly polygon, c2x* transform = nullptr);

	virtual ~CuteBounds() = default;

	CuteBounds(const CuteBounds&) = default;             // copy constructor

	CuteBounds& operator=(const CuteBounds&) = default;  // copy assignment

	CuteBounds(CuteBounds&&) = default;                  // move constructor

	CuteBounds& operator=(CuteBounds&&) = default;       // move assignment

	bool collides(float x, float y);

	bool collides(CuteBounds other);

	c2x get_transform();
private:
	C2_TYPE m_type{C2_TYPE_NONE};
	union c2Data {
		c2Circle circle;
		c2AABB aabb;
		c2Capsule capsule;
		c2Poly polygon;
	} m_structure{};
	c2x* m_transform;
};

} // namespace engine::interface

#endif //ENGINE_CUTE_BOUNDS_H
