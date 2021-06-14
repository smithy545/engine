//
// Created by Philip Smith on 6/14/21.
//

#ifndef CIVILWAR_STEADICAM_H
#define CIVILWAR_STEADICAM_H

#include "Camera.h"
#include <glm/gtx/transform.hpp>


namespace engine {
    struct Steadicam : public Camera {
        glm::vec3 position{0, 10, 0};
        bool filming{false};

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


#endif //CIVILWAR_STEADICAM_H
