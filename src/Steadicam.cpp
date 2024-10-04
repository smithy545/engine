/* Created by Philip Smith on 7/16/21.
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

#include <engine/render/camera/Steadicam.h>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>


namespace engine::render {

Steadicam::Steadicam(glm::vec3 position, glm::vec3 forward, glm::vec3 up)
		: position(position), forward(glm::normalize(forward)), up(glm::normalize(up)) {}

glm::vec3 Steadicam::get_mouse_world_coords(glm::vec2 pos) {
	auto left = glm::cross(up, forward);
	return position - left * pos.x + up * pos.y;
}

glm::vec3 Steadicam::get_forward_direction() const {
    return forward;
}

glm::vec3 Steadicam::get_position() const  {
    return position;
}

glm::mat4 Steadicam::get_view() const  {
    return glm::lookAt(position, position + forward, up);
}

void Steadicam::turn_horizontal(float diff)  {
    forward = glm::normalize(glm::rotate(forward, 0.001f * diff, up));
}

void Steadicam::turn_vertical(float diff)  {
    auto left = glm::cross(up, forward);
    forward = glm::normalize(glm::rotate(forward, diff, left));
}

void Steadicam::pan_forward(float scale)  {
    position += scale * forward;
}

void Steadicam::pan_left(float scale) {
	position -= scale * glm::cross(forward, up);
}

void Steadicam::pan_up(float scale) {
	position += scale * up;
}

} // namespace engine::render
