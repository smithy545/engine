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

#include <engine/render/camera/OrbitCam.h>

#include <glm/gtx/rotate_vector.hpp>


namespace engine {

OrbitCam::OrbitCam(glm::vec3 position, glm::vec3 focal_point)
        : position(position), focal_point(focal_point) {}

glm::vec3 OrbitCam::get_mouse_world_coords(glm::vec2 pos) {
    auto forward = get_forward_direction();
    auto right = glm::normalize(glm::cross(forward, up));
    return position + right*pos.x + glm::cross(right, forward)*pos.y;
}

glm::vec3 OrbitCam::get_forward_direction() const {
    return glm::normalize(focal_point - position);
}

glm::vec3 OrbitCam::get_position() const {
    return position;
}

glm::mat4 OrbitCam::get_view() const {
    return  glm::lookAt(position, focal_point, up);
}

void OrbitCam::move_toward(float scale) {
    auto forward = glm::normalize(focal_point - position);
    position += scale * forward;
}

void OrbitCam::move_forward(float scale) {
    auto forward = get_forward_direction();
    forward.y = 0;
    focal_point += scale * forward;
    position += scale * forward;
}

void OrbitCam::move_left(float scale) {
    position -= scale * glm::cross(get_forward_direction(), up);
}

void OrbitCam::move_up(float scale) {
    position += scale * up;
}

void OrbitCam::pan_horizontal(float diff) {
    auto dv = glm::length(focal_point - position);
    auto forward = glm::rotate(get_forward_direction(), diff, up);
    focal_point = forward*dv + position;
}

void OrbitCam::pan_vertical(float diff) {
    // TODO: make this do something
    vertical_rotation += diff;
}
} // namespace engine