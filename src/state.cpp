/* Created by Philip Smith on 5/25/21.
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

#include <engine/render/renderer.h>
#include <engine/state.h>
#include <iostream>
#include <utility>
#include <utils/graph_traversal.h>


namespace engine::state {

namespace { // pseudo-member namespace

bool s_keys[GLFW_KEY_LAST]{};
KeyHandlerChain s_key_input_chain{};
MouseButtonHandlerChain s_mouse_button_chain{};
MouseMotionHandlerChain s_mouse_motion_chain{};
MouseWheelHandlerChain s_mouse_wheel_chain{};

void set_mouse_position(double x, double y) {
	entt::monostate<PREV_MOUSE_X_KEY>{} = ((double) entt::monostate<MOUSE_X_KEY>{});
	entt::monostate<PREV_MOUSE_Y_KEY>{} = ((double) entt::monostate<MOUSE_Y_KEY>{});
	entt::monostate<MOUSE_X_KEY>{} = x;
	entt::monostate<MOUSE_Y_KEY>{} = y;
	s_mouse_motion_chain.handle(MouseMotionEvent{x, y});
}

void set_mouse_button(int button, bool value) {
	if (button == GLFW_MOUSE_BUTTON_LEFT)
		entt::monostate<MOUSE_LEFT_KEY>{} = value;
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
		entt::monostate<MOUSE_RIGHT_KEY>{} = value;
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
		entt::monostate<MOUSE_MIDDLE_KEY>{} = value;
	s_mouse_button_chain.handle(MouseButtonEvent{get_mouse_x(), get_mouse_y(), button, value});
}

void set_mouse_scroll(double yoffset) {
	entt::monostate<MOUSE_SCROLL_KEY>{} = yoffset;
	s_mouse_wheel_chain.handle(MouseWheelEvent{yoffset});
}

void set_key(int key, bool value) {
	Expects(key >= 0);
	Expects(key < GLFW_KEY_LAST);
	s_keys[key] = value;
	s_key_input_chain.handle(KeyEvent{key, value});
}


void key_cb(GLFWwindow *window, int key, int scancode, int action, int mods) {
	switch (action) {
		case GLFW_PRESS:
			set_key(key, true);
			break;
		case GLFW_RELEASE:
			set_key(key, false);
			break;
		case GLFW_REPEAT:
			break;
		default:
			std::cerr << "Key action \"" << action << "\" not handled" << std::endl;
	}
}

void cursor_pos_cb(GLFWwindow *window, double xpos, double ypos) {
	set_mouse_position(xpos, ypos);
}

void mouse_wheel_cb(GLFWwindow *window, double xoffset, double yoffset) {
	set_mouse_scroll(yoffset);
}

void mouse_button_cb(GLFWwindow *window, int button, int action, int mods) {
	switch (action) {
		case GLFW_PRESS:
			set_mouse_button(button, true);
			break;
		case GLFW_RELEASE:
			set_mouse_button(button, false);
			break;
		default:
			std::cerr << "Mouse action \"" << action << "\" not handled" << std::endl;
	}
}

void resize_cb(GLFWwindow *window, int width, int height) {
	entt::monostate<WIDTH_KEY>{} = width;
	entt::monostate<HEIGHT_KEY>{} = height;
	entt::monostate<RESIZED_KEY>{} = true;
}

} // anonymous

bool init() {
	auto window = render::get_window();

	for (auto &k: s_keys)
		k = false;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetKeyCallback(window, key_cb);
	glfwSetCursorPosCallback(window, cursor_pos_cb);
	glfwSetScrollCallback(window, mouse_wheel_cb);
	glfwSetMouseButtonCallback(window, mouse_button_cb);
	glfwSetFramebufferSizeCallback(window, resize_cb);

	// set initial window size
	int width{0}, height{0};
	glfwGetFramebufferSize(window, &width, &height);
	entt::monostate<WIDTH_KEY>{} = width;
	entt::monostate<HEIGHT_KEY>{} = height;

	return true;
}

void poll() {
	auto window = render::get_window();
	glfwPollEvents();
	reset_prev_mouse_coords();
	if(glfwWindowShouldClose(window) || get_key(GLFW_KEY_ESCAPE))
		stop();
}

double get_mouse_x() {
    return entt::monostate<MOUSE_X_KEY>{};
}

double get_mouse_y() {
    return entt::monostate<MOUSE_Y_KEY>{};
}

double get_prev_mouse_x() {
    return entt::monostate<PREV_MOUSE_X_KEY>{};
}

double get_prev_mouse_y() {
    return entt::monostate<PREV_MOUSE_Y_KEY>{};
}

void reset_prev_mouse_coords() {
    entt::monostate<PREV_MOUSE_X_KEY>{} = get_mouse_x();
    entt::monostate<PREV_MOUSE_Y_KEY>{} = get_mouse_y();
}

void start() {
    entt::monostate<START_KEY>{} = true;
    entt::monostate<STOP_KEY>{} = false;
    entt::monostate<PAUSE_KEY>{} = false;
}

void pause() {
    entt::monostate<PAUSE_KEY>{} = true;
}

void unpause() {
    entt::monostate<PAUSE_KEY>{} = false;
}

void stop() {
    entt::monostate<STOP_KEY>{} = true;
}

bool is_stopped() {
    return entt::monostate<STOP_KEY>{};
}

bool is_paused() {
    return entt::monostate<PAUSE_KEY>{};
}

bool has_started() {
    return entt::monostate<START_KEY>{};
}

bool get_key(int key) {
	return s_keys[key];
}

bool get_mouse_button(int button) {
	if (button == GLFW_MOUSE_BUTTON_LEFT)
		return entt::monostate<MOUSE_LEFT_KEY>{};
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
		return entt::monostate<MOUSE_RIGHT_KEY>{};
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
		return entt::monostate<MOUSE_MIDDLE_KEY>{};
	return false;
}

int get_width() {
    return entt::monostate<WIDTH_KEY>{};
}

int get_height() {
    return entt::monostate<HEIGHT_KEY>{};
}

bool has_resized() {
    return entt::monostate<RESIZED_KEY>{};
}

void clear_resize() {
    entt::monostate<RESIZED_KEY>{} = false;
}

KeyHandlerNode register_key_input_handler(KeyCallback callback) {
	return s_key_input_chain.set_next(std::move(callback));
}

MouseButtonHandlerNode register_mouse_button_handler(MouseButtonCallback callback) {
	return s_mouse_button_chain.set_next(std::move(callback));
}

MouseMotionHandlerNode register_mouse_motion_handler(MouseMotionCallback callback) {
	return s_mouse_motion_chain.set_next(std::move(callback));
}

MouseWheelHandlerNode register_mouse_wheel_handler(MouseWheelCallback callback) {
	return s_mouse_wheel_chain.set_next(std::move(callback));
}

void unregister_key_input_handler(KeyHandlerNode node) {
	s_key_input_chain.remove(std::move(node));
}

void unregister_mouse_button_handler(MouseButtonHandlerNode node) {
	s_mouse_button_chain.remove(std::move(node));
}

void unregister_mouse_motion_handler(MouseMotionHandlerNode node) {
	s_mouse_motion_chain.remove(std::move(node));
}

void unregister_mouse_wheel_handler(MouseWheelHandlerNode node) {
	s_mouse_wheel_chain.remove(std::move(node));
}

} // namespace engine::state
