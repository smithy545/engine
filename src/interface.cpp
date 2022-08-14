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
#include <engine/interface/Bounds.h>
#include <engine/interface/interface.h>
#include <engine/interface/EntityMap.h>
#include <engine/render/renderer.h>
#include <engine/state.h>
#include <entt/entt.hpp>
#include <map>
#include <set>


namespace engine::interface {

namespace {

// EntityMap maps screen position to the nearest interface entity by object center
// Overlapping objects or 3D objects require more complex collision detection TBA
EntityMap entity_locator;
std::map<entt::entity, Widget::Ptr> loaded_widgets;
std::set<entt::entity> button_widgets, wheel_widgets, key_widgets;
entt::entity hover_entity{entt::null}, focused_entity{entt::null};
entt::registry registry;

} // anonymous

bool init() {
	// collision handling via cute_c2
	registry.on_construct<Bounds>().connect<&construct_bounds>();
	registry.on_destroy<Bounds>().connect<&destroy_bounds>();

	state::register_key_input_handler([&](KeyEvent event) {
		if(focused_entity != entt::null) {
			loaded_widgets[focused_entity]->publish<KeyEvent>(event);
			return false;
		}
		return true;
	});
	state::register_mouse_button_handler([&](MouseButtonEvent event) {
		if(hover_entity != entt::null && event.button == GLFW_PRESS)
			focused_entity = hover_entity;
		if(focused_entity != entt::null) {
			loaded_widgets[focused_entity]->publish<MouseButtonEvent>(event);
			return false;
		}
		return true;
	});
	state::register_mouse_motion_handler([&](MouseMotionEvent event) {
		auto closest = entity_locator.closest_n(event.x, event.y, 3);
		for(auto entity: closest) {
			auto& bounds = registry.get<Bounds>(entity);
			if(bounds.collides(event.x, event.y)) {
				hover_entity = entity;
				return false;
			}
		}
		return true;
	});
	state::register_mouse_wheel_handler([&](MouseWheelEvent event) {
		if(focused_entity != entt::null) {
			loaded_widgets[focused_entity]->publish<MouseWheelEvent>(event);
			return false;
		}
		return true;
	});

	return true;
}

void cleanup() {
	registry.clear();
}

entt::entity create_widget(const EnttMouseButtonCallback& click_cb,
						   const EnttMouseWheelCallback& scroll_cb,
						   const EnttKeyCallback& key_cb) {
	auto widget = Widget::create(render::get_registry(), click_cb, scroll_cb, key_cb);
	auto entity = widget->get_entity();
	loaded_widgets[entity] = widget;
	if(widget->is_clickable())
		button_widgets.insert(entity);
	if(widget->is_scrollable())
		wheel_widgets.insert(entity);
	if(widget->is_typable())
		key_widgets.insert(entity);
	return entity;
}

entt::entity attach_widget(const gsl::not_null<Widget::Ptr>& widget) {
	auto entity = widget->get_entity();
	loaded_widgets[entity] = widget;
	if(widget->is_clickable())
		button_widgets.insert(entity);
	if(widget->is_scrollable())
		wheel_widgets.insert(entity);
	if(widget->is_typable())
		key_widgets.insert(entity);
	return entity;
}

void construct_bounds(entt::registry& registry, entt::entity entity) {
	auto& bounds = registry.get<Bounds>(entity);
	entity_locator.put(bounds.get_position(), entity);
}

void destroy_bounds(entt::registry& registry, entt::entity entity) {
	auto& bounds = registry.get<Bounds>(entity);
	entity_locator.erase(bounds.get_position(), entity);
}

} // namespace engine::interface