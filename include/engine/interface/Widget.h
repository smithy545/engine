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
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EBoundsPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef ENGINE_WIDGET_H
#define ENGINE_WIDGET_H

#include <chrono>
#include <engine/event_handling.h>
#include <engine/interface/CuteBounds.h>
#include <engine/interface/interface.h>
#include <entt/entt.hpp>



namespace engine::interface {

// generic interface entity
class Widget : public interface::CuteBounds {
public:
	explicit Widget(c2AABB bounds, c2x* transform = nullptr);

    ~Widget();

protected:
    entt::entity m_id{entt::null};

    virtual bool on_tick(std::chrono::nanoseconds dt) = 0;

    virtual bool on_mouse_button(MouseButtonEvent event) = 0;

    virtual bool on_mouse_motion(MouseMotionEvent event) = 0;

    virtual bool on_key(KeyEvent event) = 0;

    // parameters allow use of empty functions to replace pure functions without adding empty function calls to event handlers
    void register_entt_callbacks(bool set_tick = true, bool set_button = true, bool set_motion = true, bool set_key = true);

};

} // namespace engine::interface

#endif //ENGINE_WIDGET_H
