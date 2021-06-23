//
// Created by Philip Smith on 6/14/21.
//

#ifndef ENGINE_STEADICAM_H
#define ENGINE_STEADICAM_H

#include "Camera.h"
#include <glm/gtx/transform.hpp>


namespace engine {
    struct Steadicam : public Camera {
        glm::vec3 position;
        bool filming;

        Steadicam(glm::vec3 position, bool filming = false)
        : position(position), filming(filming) {}

        glm::vec3 get_mouse_world_coords(glm::vec2 pos) override {
            auto left = glm::cross(up, forward);
            return position - left*pos.x + up*pos.y;
        }

        glm::vec3 get_forward_direction() const override {
            return forward;
        }

        glm::vec3 get_position() const override {
            return position;
        }

        glm::mat4 get_view() const override {
            return glm::lookAt(position, position + forward, up);
        }

        void pan_horizontal(float diff) override {
            forward = glm::normalize(glm::rotate(forward, 0.001f * diff, up));
        }

        void pan_vertical(float diff) override {
            auto left = glm::cross(up, forward);
            forward = glm::normalize(glm::rotate(forward, 0.001f * diff, left));
        }

        void move_forward(float scale = 1.0f) override {
            position.x += scale * forward.x;
            position.z += scale * forward.z;
        }

        void move_backward(float scale = 1.0f) override {
            position.x -= scale * forward.x;
            position.z -= scale * forward.z;
        }

        void move_left(float scale = 1.0f) override {
            position -= scale * glm::cross(forward, up);
        }

        void move_right(float scale = 1.0f) override {
            position += scale * glm::cross(forward, up);
        }

        void move_up(float scale = 1.0f) override {
            position += scale * up;
        }

        void move_down(float scale = 1.0f) override {
            position -= scale * up;
        }
    protected:
        glm::vec3 forward{0, 0, 1};
        glm::vec3 up{0, 1, 0};
    };
} // namespace engine


#endif //ENGINE_STEADICAM_H
