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

#include <engine/interface/EntityMap.h>


namespace engine::interface {

EntityMap::EntityMap(vector<Point_2> points) : m_points(std::move(points)) {
	for(auto p: m_points)
		m_value_map[p] = entt::null;
}

entity EntityMap::operator[](const Point_2& query) {
	return m_value_map.at(utils::math::query_closest(m_points, query));
}

entity EntityMap::at(double x, double y) {
	if(m_value_map.empty())
		return entt::null;
	return m_value_map.at(utils::math::query_closest(m_points, Point_2(x, y)));
}

const map<Point_2, entity>& EntityMap::get_value_map() const {
	return m_value_map;
}

const vector<Point_2>& EntityMap::get_points() const {
	return m_points;
}

void EntityMap::put(const Point_2& query, entity ent) {
	if(!m_value_map.contains(query))
		m_points.push_back(query);
	m_value_map[query] = ent;
}

void EntityMap::erase(const Point_2& query) {
	if(m_value_map.contains(query))
		m_value_map.erase(query);
	int index{0};
	for(auto p: m_points) {
		if (p == query)
			break;
		index++;
	}
	if(index < m_points.size())
		m_points.erase(m_points.begin() + index);
}

} // namespace engine::interface
