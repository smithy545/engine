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

#include <engine/event_handling.h>
#include <engine/interface/interface.h>
#include <engine/interface/EntityMap.h>
#include <engine/state.h>
#include <entt/entt.hpp>
#include <map>
#include <set>
#include <vector>


namespace engine::interface {

namespace {

// EntityMap maps screen position to the nearest interface entity by object center
// TODO: Check for collision upon adding object to entity map and keep track of overlapping entities
EntityMap s_entity_locator;
std::set<entt::entity> s_loaded_widgets;
entt::entity s_nearest_entity{entt::null}, s_focused_entity{entt::null};
entt::registry s_registry;

// local utility functions
void construct_bounds(entt::registry& registry, entt::entity entity) {
	auto& bounds = registry.get<CuteBounds>(entity);
	auto tx = bounds.get_transform();
	s_entity_locator.put(Point_2(tx.p.x, tx.p.y), entity);
	s_loaded_widgets.insert(entity);
}

void destroy_bounds(entt::registry& registry, entt::entity entity) {
	auto& bounds = registry.get<CuteBounds>(entity);
	auto transform = bounds.get_transform();
	s_entity_locator.erase(Point_2(transform.p.x, transform.p.y), entity);
	s_loaded_widgets.erase(entity);
}

} // anonymous

bool init() {
	// collision handling via cute_c2
	s_registry.on_construct<CuteBounds>().connect<&construct_bounds>();
	s_registry.on_destroy<CuteBounds>().connect<&destroy_bounds>();

	// apply keystrokes and mouse actions only to focused entity
	// create new event objects to avoid modification out of function
	state::register_key_input_handler([&](KeyEvent event) {
		if(s_focused_entity != entt::null) {
			auto cb = s_registry.try_get<KeyCallback>(s_focused_entity);
			if(cb)
				(*cb)(KeyEvent(event));
			return false;
		}
		return true;
	});

	state::register_mouse_button_handler([&](MouseButtonEvent event) {
		if(s_focused_entity != entt::null) {
			// check collision
			auto bounds = s_registry.get<CuteBounds>(s_focused_entity);
			if(!bounds.collides(event.x, event.y))
				return true;

			// on collision call mouse button handler
			auto cb = s_registry.try_get<MouseButtonCallback>(s_focused_entity);
			if(cb)
				return (*cb)(MouseButtonEvent(event));
		}
		return true;
	});

	// use mouse motion to update nearest entities
	state::register_mouse_motion_handler([&](MouseMotionEvent event) {
		// TODO: handle case where the center of the nearest entity does not correspond to the entity that is "nearest"
		s_nearest_entity = *s_entity_locator.at(event.x, event.y).begin();

		if(s_focused_entity != entt::null) {
			auto cb = s_registry.try_get<MouseMotionCallback>(s_focused_entity);
			if(cb)
				return (*cb)(MouseMotionEvent(event));
		}

		return true;
	});

	state::register_mouse_wheel_handler([&](MouseWheelEvent event) {
		if(s_focused_entity != entt::null) {
			auto cb = s_registry.try_get<MouseWheelCallback>(s_focused_entity);
			if(cb)
				return (*cb)(MouseWheelEvent(event));
		}
		return true;
	});

	return true;
}

void cleanup() {
	s_registry.clear();
}

entt::registry& get_registry() {
	return s_registry;
}

entt::entity get_nearest_entity() {
	return s_nearest_entity;
}

entt::entity get_focused_entity() {
	return s_focused_entity;
}

void focus(entt::entity entity) {
	s_focused_entity = entity;
}

void tick(std::chrono::nanoseconds dt) {
	auto view = s_registry.view<TickCallback>();
	for(auto entity: view)
		view.get<TickCallback>(entity)(dt);
}

} // namespace engine::interface