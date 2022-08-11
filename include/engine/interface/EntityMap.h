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

#ifndef ENGINE_ENTITYMAP_H
#define ENGINE_ENTITYMAP_H

#include <entt/entt.hpp>
#include <map>
#include <utils/math_util.h>
#include <vector>


using entt::entity;
using std::map;
using std::vector;
using utils::math::Point_2;

namespace engine::interface {

class EntityMap {
public:
	EntityMap() = default;

	explicit EntityMap(vector<Point_2> points);

	virtual ~EntityMap() = default;

	entity operator[](const Point_2& query);

	entity at(double x, double y);

	[[nodiscard]] const map<Point_2, entity>& get_value_map() const;

	[[nodiscard]] const vector<Point_2>& get_points() const;

	void put(const Point_2& query, entity ent);

	void erase(const Point_2& query);
private:
	vector<Point_2> m_points{};
	map<Point_2, entity> m_value_map{};
};

} // namespace engine::interface

#endif //ENGINE_ENTITYMAP_H
