//
// Created by Philip Smith on 5/18/21.
//

#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>


namespace engine {
    struct Camera {
        glm::vec3 position{0, 10, 0};
        glm::vec3 forward{0, 0, 1};
        glm::vec3 up{0, 1, 0};
        bool filming{false};

        void pan_horizontal(float diff) {
            forward = glm::normalize(glm::rotate(forward, 0.001f * diff, up));
        }

        void pan_vertical(float diff) {
            auto left = glm::cross(up, forward);
            forward = glm::normalize(glm::rotate(forward, 0.001f * diff, left));
        }

        void move_forward(float scale = 1.0f) {
            position.x += scale * forward.x;
            position.z += scale * forward.z;
        }

        void move_backward(float scale = 1.0f) {
            position.x -= scale * forward.x;
            position.z -= scale * forward.z;
        }

        void move_left(float scale = 1.0f) {
            position -= scale * glm::cross(forward, up);
        }

        void move_right(float scale = 1.0f) {
            position += scale * glm::cross(forward, up);
        }

        void move_up(float scale = 1.0f) {
            position += scale * up;
        }

        void move_down(float scale = 1.0f) {
            position -= scale * up;
        }

        glm::mat4 get_view() const {
            return  glm::lookAt(position, position + forward, up);
        }
    };
} // namespace engine

#endif //ENGINE_CAMERA_H
