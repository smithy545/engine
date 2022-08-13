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

#include <engine/interface/Widget.h>


namespace engine::interface {

Widget::Widget(entt::registry& registry) : m_entity(registry.create()) {}

Widget::~Widget() {
	clear();
}

void Widget::attach_mouse_click_handler(const EnttMouseButtonCallback& callback) {
	this->on<MouseButtonEvent>([callback](const MouseButtonEvent& event, Widget& emitter) {
		callback(event);
	});
}

void Widget::attach_mouse_wheel_handler(const EnttMouseWheelCallback& callback) {
	this->on<MouseWheelEvent>([callback](const MouseWheelEvent& event, Widget& emitter) {
		callback(event);
	});}

void Widget::attach_key_handler(const EnttKeyCallback& callback) {
	this->on<KeyEvent>([callback](const KeyEvent& event, Widget& emitter) {
		callback(event);
	});
}

bool Widget::is_clickable() {
	return !this->empty<MouseButtonEvent>();
}

bool Widget::is_scrollable() {
	return !this->empty<MouseWheelEvent>();
}

bool Widget::is_typable() {
	return !this->empty<KeyEvent>();
}

bool Widget::is_visible() {
	return m_visible;
}

void Widget::set_visible(bool val) {
	m_visible = val;
}

Widget::Ptr Widget::create(entt::registry& registry) {
	return Ptr(new Widget(registry));
}

} // namespace engine::interface