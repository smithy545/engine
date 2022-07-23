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

#ifndef ENGINE_POINTFINDER_H
#define ENGINE_POINTFINDER_H

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Quadtree.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/Orthogonal_k_neighbor_search.h>
#include <CGAL/Search_traits_2.h>
#include <fmt/format.h>
#include <string>
#include <vector>


using std::vector;

namespace engine {
typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;
#ifdef USE_QUADTREE_COLLISION
typedef CGAL::Quadtree<Kernel, vector<Point_2>> Quadtree;
#else
typedef CGAL::Search_traits_2<Kernel> TreeTraits;
typedef CGAL::Orthogonal_k_neighbor_search<TreeTraits> Neighbor_search;
#endif

// Convenience wrapper for quadtree/kdtree
class PointFinder {
public:
	explicit PointFinder(vector<Point_2> sites);

	explicit PointFinder(vector<double> coords);

	Point_2 operator[](const Point_2& query) const;

	[[nodiscard]] Point_2 find_closest(const Point_2& query) const;

	[[nodiscard]] const vector<Point_2>& get_points() const;
private:
	vector<Point_2> m_points{};
#ifdef USE_QUADTREE_COLLISION
	Quadtree m_collision_tree;
#endif
	static vector<Point_2> convert_to_point_2(vector<double> coords);
};

vector<Point_2> generate_sites(unsigned int num_regions, unsigned int width, unsigned int height);
} // namespace engine

namespace std {
template <>
struct hash<engine::Point_2> {
	size_t operator()(const engine::Point_2& p) const {
		return hash<std::string>()(fmt::format("{}:{}", p.x(), p.y()));
	}
};
} // namespace std


#endif //ENGINE_POINTFINDER_H
