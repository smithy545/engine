/* Created by Philip Smith on 1/12/22.
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

#include <engine/PointFinder.h>

#include <gsl/gsl>
#include <random>
#include <unordered_set>


namespace engine {
PointFinder::PointFinder(vector<Point_2> sites) : m_points(std::move(sites)) {
#ifdef USE_QUADTREE_COLLISION
	m_collision_tree(m_points);
    m_collision_tree.refine();
#endif
}

PointFinder::PointFinder(vector<double> coords) : PointFinder(convert_to_point_2(std::move(coords))) {}

Point_2 PointFinder::operator[](const Point_2& query) const {
	Point_2 target;
#ifndef USE_QUADTREE_COLLISION
	Neighbor_search::Tree tree(m_points.begin(), m_points.end());
	Neighbor_search search(tree, query, 1);
	target = search.begin()->first;
#else
	m_collision_tree.nearest_neighbors(query, 1, boost::make_function_output_iterator([&](const Point_2& k) {
	    target = k;
    }));
#endif
	return target;
}

Point_2 PointFinder::find_closest(const Point_2& query) const {
	Point_2 target{query};
#ifndef USE_QUADTREE_COLLISION
	Neighbor_search::Tree tree(m_points.begin(), m_points.end());
	Neighbor_search search(tree, query, 1);
	target = search.begin()->first;
#else
	m_collision_tree.nearest_neighbors(query, 1, boost::make_function_output_iterator([&](const Point_2& k) {
        target = k;
    }));
#endif
	return target;
}

const vector<Point_2> &PointFinder::get_points() const {
	return m_points;
}

vector<Point_2> PointFinder::convert_to_point_2(vector<double> coords) {
	Expects(coords.size() % 2 == 0);
	vector<Point_2> points;
	for(auto i = 0; i < coords.size(); i+= 2)
		points.emplace_back(coords[i], coords[i+1]);
	return points;
}

vector<Point_2> generate_sites(unsigned int num_regions, unsigned int width, unsigned int height) {
	Expects(width > 0);
	Expects(height > 0);
	std::random_device rd;
	std::unordered_set<Point_2> points;
	std::vector<Point_2> sites;
	while (points.size() < num_regions) {
		Point_2 v;
		do {
			v = Point_2(rd() % width, rd() % height);
		} while (points.contains(v));
		points.insert(v);
		sites.push_back(v);
	}
	return sites;
}
} // namespace engine
