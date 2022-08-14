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

#ifndef ENGINE_WIDGET_H
#define ENGINE_WIDGET_H

#include <engine/event_handling.h>
#include <entt/entt.hpp>
#include <utils/macros.h>


namespace engine::interface {

class Widget : public entt::emitter<Widget> {
public:
	PTR(Widget);

	friend entt::entity create_widget(const EnttMouseButtonCallback& click_cb,
	                                  const EnttMouseWheelCallback& scroll_cb,
	                                  const EnttKeyCallback& key_cb);

	friend entt::entity attach_widget(const gsl::not_null<Widget::Ptr>& widget);

	Widget() = delete;

	~Widget() override;

	bool is_clickable();

	bool is_scrollable();

	bool is_typable();
protected:
	explicit Widget(entt::registry& registry);

	void attach_mouse_click_handler(const EnttMouseButtonCallback& callback);

	void attach_mouse_wheel_handler(const EnttMouseWheelCallback& callback);

	void attach_key_handler(const EnttKeyCallback& callback);

	entt::entity get_entity();

private:
	static Ptr create(
			entt::registry& registry,
			const EnttMouseButtonCallback& click_cb = nullptr,
			const EnttMouseWheelCallback& scroll_cb = nullptr,
			const EnttKeyCallback& key_cb = nullptr);

	entt::entity m_entity{entt::null};
};

} // namespace engine::interface

#endif //ENGINE_WIDGET_H
