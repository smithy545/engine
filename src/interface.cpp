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

#include <boost/graph/adjacency_list.hpp>
#include <engine/interface/interface.h>
#include <engine/state.h>
#include <memory>


using namespace utils;

namespace engine::interface {

namespace {

using IGraph = boost::adjacency_list<boost::listS, boost::setS, boost::directedS, InterfaceNode::Ptr>;
using IGraphTraits = boost::graph_traits<IGraph>;
IGraph graph;
IGraphTraits::vertex_descriptor root_descriptor;

} // anonymous

bool init(entt::registry &registry) {
	auto node = std::make_shared<InterfaceNode>(math::bounds{{0.,0.},{0.,0.}});
	root_descriptor = boost::add_vertex(node, graph);

	state::register_key_input_handler([&](KeyEvent event) {
		return true;
	});
	state::register_mouse_button_handler([&](MouseButtonEvent event) {
		return true;
	});
	state::register_mouse_motion_handler([&](MouseMotionEvent event) {
		return true;
	});
	state::register_mouse_wheel_handler([&](MouseWheelEvent event) {
		return true;
	});

	return true;
}

void cleanup(entt::registry &registry) {}

InterfaceNode::Ptr root() {
	return graph[root_descriptor];
}

} // namespace engine::interface