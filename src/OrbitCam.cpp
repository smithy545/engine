//
// Created by Philip Smith on 7/16/21.
//

#include <engine/OrbitCam.h>


namespace engine {

    OrbitCam::OrbitCam(glm::vec3 position, glm::vec3 focal_point, bool filming)
            : position(position), focal_point(focal_point), filming(filming) {}

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

    void OrbitCam::move_away(float scale) {
        auto forward = glm::normalize(focal_point - position);
        position -= scale * forward;
    }

    void OrbitCam::move_forward(float scale) {
        auto forward = get_forward_direction();
        forward.y = 0;
        focal_point += scale * forward;
        position += scale * forward;
    }

    void OrbitCam::move_backward(float scale) {
        auto forward = get_forward_direction();
        forward.y = 0;
        focal_point -= scale * forward;
        position -= scale * forward;
    }

    void OrbitCam::move_left(float scale) {
        position -= scale * glm::cross(get_forward_direction(), up);
    }

    void OrbitCam::move_right(float scale) {
        position += scale * glm::cross(get_forward_direction(), up);
    }

    void OrbitCam::move_up(float scale) {
        position += scale * up;
    }

    void OrbitCam::move_down(float scale) {
        position -= scale * up;
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