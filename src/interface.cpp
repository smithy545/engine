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
#include <engine/render/InstanceList.h>
#include <engine/render/sprite/ShapeSprite.h>
#include <engine/state.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace engine::interface {

namespace {

// EntityMap maps screen position to the nearest interface entity by object center
// Overlapping objects or 3D objects require more complex collision detection TBA
std::unique_ptr<EntityMap> locator{nullptr};
entt::entity active_entity{entt::null};

} // anonymous

bool init(entt::registry& registry) {
	locator = std::make_unique<EntityMap>();
	active_entity = registry.create();
	ShapeSprite sprite;
	sprite.vertices = {
			glm::vec2(0,0),
			glm::vec2(100,0),
			glm::vec2(0,100),
			glm::vec2(100,100),
	};
	sprite.indices = {
			1, 0, 2, 3, 1, 2
	};
	sprite.colors = {
			glm::vec3(100,0,0),
			glm::vec3(100,0,0),
			glm::vec3(100,0,0),
			glm::vec3(100,0,0),
	};
	registry.emplace<ShapeSprite>(active_entity, sprite);
	registry.patch<InstanceList>(active_entity, [](auto& instances) {
		instances.add_instance(glm::mat4(1));
		instances.add_instance(glm::translate(glm::mat4(1), glm::vec3(700,0,0)));
		instances.add_instance(glm::translate(glm::mat4(1), glm::vec3(0,500,0)));
		instances.add_instance(glm::translate(glm::mat4(1), glm::vec3(700,500,0)));
	});

	state::register_key_input_handler([&](KeyEvent event) {
		return true;
	});
	state::register_mouse_button_handler([&](MouseButtonEvent event) {
		return true;
	});
	state::register_mouse_motion_handler([&](MouseMotionEvent event) {
		//active_entity = locator->at(event.x, event.y);
		return true;
	});
	state::register_mouse_wheel_handler([&](MouseWheelEvent event) {
		return true;
	});

	return true;
}

void cleanup(entt::registry& registry) {}

} // namespace engine::interface