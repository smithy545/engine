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

#ifndef ENGINE_STATE_H
#define ENGINE_STATE_H

#include <engine/event_handling.h>
#include <engine/render/RenderTreeNode.h>
#include <entt/entt.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


namespace engine::state {
constexpr entt::hashed_string MOUSE_X_KEY{"mouse_x"};
constexpr entt::hashed_string MOUSE_Y_KEY{"mouse_y"};
constexpr entt::hashed_string PREV_MOUSE_X_KEY{"prev_mouse_x"};
constexpr entt::hashed_string PREV_MOUSE_Y_KEY{"prev_mouse_y"};
constexpr entt::hashed_string MOUSE_SCROLL_KEY{"mouse_scroll"};
constexpr entt::hashed_string MOUSE_LEFT_KEY{"mouse_left"};
constexpr entt::hashed_string MOUSE_MIDDLE_KEY{"mouse_middle"};
constexpr entt::hashed_string MOUSE_RIGHT_KEY{"mouse_right"};
constexpr entt::hashed_string RESIZED_KEY{"resized"};
constexpr entt::hashed_string WIDTH_KEY{"width"};
constexpr entt::hashed_string HEIGHT_KEY{"height"};
constexpr entt::hashed_string PAUSE_KEY{"paused"};
constexpr entt::hashed_string STOP_KEY{"stopped"};
constexpr entt::hashed_string START_KEY{"started"};

using KeyHandlerNode = KeyHandlerChain::HandlerNode::Ptr;
using MouseButtonHandlerNode = MouseButtonHandlerChain::HandlerNode::Ptr;
using MouseMotionHandlerNode = MouseMotionHandlerChain::HandlerNode::Ptr;
using MouseWheelHandlerNode = MouseWheelHandlerChain::HandlerNode::Ptr;

bool init();

void tick();

void render();

void poll();

void cleanup();

entt::registry& get_registry();

double get_mouse_x();

double get_mouse_y();

double get_prev_mouse_x();

double get_prev_mouse_y();

bool get_mouse_button(int button);

bool get_key(int key);

int get_width();

int get_height();

bool has_resized();

void clear_resize();

void reset_prev_mouse_coords();

void start();

void pause();

void unpause();

void stop();

bool is_stopped();

bool is_paused();

bool has_started();

void register_input_callbacks(GLFWwindow* window);

KeyHandlerNode register_key_input_handler(entt::poly<KeyHandler> handler);

void unregister_key_input_handler(KeyHandlerChain::HandlerNode::Ptr handler);

MouseButtonHandlerNode register_mouse_button_handler(entt::poly<MouseButtonHandler> handler);

void unregister_mouse_button_handler(MouseButtonHandlerNode handler);

MouseMotionHandlerNode register_mouse_motion_handler(entt::poly<MouseMotionHandler> handler);

void unregister_mouse_motion_handler(MouseMotionHandlerNode handler);

MouseWheelHandlerNode register_mouse_wheel_handler(entt::poly<MouseWheelHandler> handler);

void unregister_mouse_wheel_handler(MouseWheelHandlerNode handler);
} // namespace engine::state



#endif //ENGINE_STATE_H
