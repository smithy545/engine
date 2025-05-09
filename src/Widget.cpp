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
#include <functional>


namespace engine::interface {

Widget::Widget(c2AABB bounds, c2x* transform) : CuteBounds(bounds, transform), m_id(interface::get_registry().create()) {
    interface::get_registry().emplace_or_replace<CuteBounds>(m_id, *this);
}

Widget::~Widget() {
    if(m_id != entt::null)
        interface::get_registry().destroy(m_id);
}

void Widget::register_entt_callbacks(bool set_tick, bool set_button, bool set_motion, bool set_key) {
    if(set_tick)
        interface::get_registry().emplace_or_replace<interface::TickCallback>(m_id,
            std::bind(&Widget::on_tick, this, std::placeholders::_1));
    if(set_button)
        interface::get_registry().emplace_or_replace<MouseButtonCallback>(m_id,
            std::bind(&Widget::on_mouse_button, this, std::placeholders::_1));
    if(set_motion)
        interface::get_registry().emplace_or_replace<MouseMotionCallback>(m_id,
            std::bind(&Widget::on_mouse_motion, this, std::placeholders::_1));
    if(set_key)
        interface::get_registry().emplace_or_replace<KeyCallback>(m_id,
            std::bind(&Widget::on_key, this, std::placeholders::_1));
}

} // namespace engine::interface