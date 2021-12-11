//
// Created by Philip Smith on 7/16/21.
//

#include <engine/Steadicam.h>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>


namespace engine {

    Steadicam::Steadicam(glm::vec3 position, glm::vec3 forward, glm::vec3 up)
    : position(position), forward(glm::normalize(forward)), up(glm::normalize(up)) {}

    glm::vec3 Steadicam::get_mouse_world_coords(glm::vec2 pos) {
        auto left = glm::cross(up, forward);
        return position - left*pos.x + up*pos.y;
    }

    glm::vec3 Steadicam::get_forward_direction() const  {
        return forward;
    }

    glm::vec3 Steadicam::get_position() const  {
        return position;
    }

    glm::mat4 Steadicam::get_view() const  {
        return glm::lookAt(position, position + forward, up);
    }

    void Steadicam::pan_horizontal(float diff)  {
        forward = glm::normalize(glm::rotate(forward, 0.001f * diff, up));
    }

    void Steadicam::pan_vertical(float diff)  {
        auto left = glm::cross(up, forward);
        forward = glm::normalize(glm::rotate(forward, 0.001f * diff, left));
    }

    void Steadicam::move_forward(float scale)  {
        position.x += scale * forward.x;
        position.z += scale * forward.z;
    }

    void Steadicam::move_left(float scale)  {
        position -= scale * glm::cross(forward, up);
    }

    void Steadicam::move_up(float scale)  {
        position += scale * up;
    }
} // namespace engine