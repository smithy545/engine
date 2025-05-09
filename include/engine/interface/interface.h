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

#ifndef ENGINE_INTERFACE_H
#define ENGINE_INTERFACE_H

#include <chrono>
#include <engine/event_handling.h>
#include <engine/interface/CuteBounds.h>
#include <entt/entt.hpp>
#include <gsl/gsl>
#include <utils/macros.h>


namespace engine::interface {

// for updating widgets on game tick
using TickCallback = EventCallback<std::chrono::nanoseconds>;

bool init();

void cleanup();

entt::registry& get_registry();

entt::entity get_nearest_entity();

entt::entity get_focused_entity();

void focus(entt::entity entity);

void tick(std::chrono::nanoseconds dt);

} // namespace engine::interface

#endif //ENGINE_INTERFACE_H
