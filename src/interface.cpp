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

#include <engine/interface/interface.h>
#include <engine/interface/EntityMap.h>
#include <engine/render/renderer.h>
#include <engine/state.h>
#include <entt/entt.hpp>
#include <map>


namespace engine::interface {

namespace {

// EntityMap maps screen position to the nearest interface entity by object center
// Overlapping objects or 3D objects require more complex collision detection TBA
EntityMap entity_locator;
std::map<entt::entity, Widget::Ptr> loaded_widgets;

} // anonymous

bool init() {
	auto& registry = render::get_registry();

	state::register_key_input_handler([&](KeyEvent event) {
		return true;
	});

	state::register_mouse_button_handler([&](MouseButtonEvent event) {
		auto mouse_pos = Point_2(event.x, event.y);
		auto entities = entity_locator.closest_n(event.x, event.y, loaded_widgets.size() < 5 ? loaded_widgets.size() : 5);
		for(auto entity: entities) {
			auto bounds = registry.get<utils::math::bounds>(entity);
			if (utils::math::in_bounds(mouse_pos, bounds)) {
				loaded_widgets[entity]->publish<MouseButtonEvent>(event);
				return true; // trigger widget
			}
		}

		return true;
	});
	state::register_mouse_motion_handler([&](MouseMotionEvent event) {
		auto mouse_pos = Point_2(event.x, event.y);
		auto entities = entity_locator.closest_n(event.x, event.y, loaded_widgets.size() < 5 ? loaded_widgets.size() : 5);
		for(auto entity: entities) {
			auto bounds = registry.get<utils::math::bounds>(entity);
			if (utils::math::in_bounds(mouse_pos, bounds)) {
				return true; // highlight widget
			}
		}

		return true;
	});
	state::register_mouse_wheel_handler([&](MouseWheelEvent event) {
		return true;
	});

	return true;
}

void cleanup() {}

entt::entity create_widget() {
	auto& registry = render::get_registry();
	auto entity = registry.create();
	loaded_widgets[entity] = Widget::create(registry);
	return entity;
}
} // namespace engine::interface