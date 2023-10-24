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
#include <engine/interface/CuteBounds.h>
#include <engine/interface/interface.h>
#include <engine/interface/EntityMap.h>
#include <engine/state.h>
#include <entt/entt.hpp>
#include <map>
#include <set>


namespace engine::interface {

namespace {

// EntityMap maps screen position to the nearest interface entity by object center
// Overlapping objects or 3D objects require more complex collision detection TBA
EntityMap s_entity_locator;
std::map<entt::entity, Widget> s_loaded_widgets;
std::set<entt::entity> s_button_widgets, s_wheel_widgets, s_key_widgets;
entt::entity s_hover_entity{entt::null}, s_focus_entity{entt::null};
entt::registry s_registry;

} // anonymous

bool init() {
	// collision handling via cute_c2
	s_registry.on_construct<CuteBounds>().connect<&construct_bounds>();
	s_registry.on_destroy<CuteBounds>().connect<&destroy_bounds>();

	state::register_key_input_handler([&](KeyEvent event) {
		if(s_focus_entity != entt::null) {
			s_loaded_widgets[s_focus_entity].publish<KeyEvent>(KeyEvent(event));
			return false;
		}
		return true;
	});
	state::register_mouse_button_handler([&](MouseButtonEvent event) {
		if(s_hover_entity != entt::null && event.button == GLFW_PRESS)
			s_focus_entity = s_hover_entity;
		if(s_focus_entity != entt::null) {
			s_loaded_widgets[s_focus_entity].publish<MouseButtonEvent>(MouseButtonEvent(event));
			return false;
		}
		return true;
	});
	state::register_mouse_motion_handler([&](MouseMotionEvent event) {
		auto closest = s_entity_locator.closest_n(event.x, event.y, 3);
		for(auto entity: closest) {
			// assume no overlap for now
			auto& bounds = s_registry.get<CuteBounds>(entity);
			if(bounds.collides(event.x, event.y)) {
				s_hover_entity = entity;
				return false;
			}
		}
		return true;
	});
	state::register_mouse_wheel_handler([&](MouseWheelEvent event) {
		if(s_focus_entity != entt::null) {
			s_loaded_widgets[s_focus_entity].publish<MouseWheelEvent>(MouseWheelEvent(event));
			return false;
		}
		return true;
	});

	return true;
}

void cleanup() {
	s_registry.clear();
}

entt::entity create_widget(const EnttMouseButtonCallback& click_cb,
						   const EnttMouseWheelCallback& scroll_cb,
						   const EnttKeyCallback& key_cb,
						   bool focus) {
	Widget widget(click_cb, scroll_cb, key_cb);
	auto entity = s_registry.create();
	if(widget.is_clickable())
		s_button_widgets.insert(entity);
	if(widget.is_scrollable())
		s_wheel_widgets.insert(entity);
	if(widget.is_typable())
		s_key_widgets.insert(entity);
	if(focus)
		s_focus_entity = entity;
	s_loaded_widgets[entity] = std::move(widget);
	return entity;
}

void focus(entt::entity entity) {
	s_focus_entity = entity;
}

void construct_bounds(entt::registry& registry, entt::entity entity) {
	auto& bounds = registry.get<CuteBounds>(entity);
	auto tx = bounds.get_transform();
	s_entity_locator.put(Point_2(tx.p.x, tx.p.y), entity);
}

void destroy_bounds(entt::registry& registry, entt::entity entity) {
	auto& bounds = registry.get<CuteBounds>(entity);
	auto transform = bounds.get_transform();
	s_entity_locator.erase(Point_2(transform.p.x, transform.p.y), entity);
}

} // namespace engine::interface