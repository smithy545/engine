//
// Created by Philip Smith on 6/14/21.
//

#ifndef CIVILWAR_ORBITCAMERA_H
#define CIVILWAR_ORBITCAMERA_H

#include "Camera.h"
#include <glm/gtx/rotate_vector.hpp>


namespace engine {
    struct OrbitCam : public Camera {
        glm::vec3 focal_point{0, 0, 0};
        glm::vec3 position{0, 10, 0};
        bool filming{false};

        glm::mat4 get_view() const override {
            if(horizontal_rotation == 0.0f)
                return  glm::lookAt(position, focal_point, up);
            auto forward = glm::normalize(focal_point - position);
            forward = glm::rotate(forward, horizontal_rotation, up);
            return  glm::lookAt(position, position + forward, up);
        }

        void move_forward(float scale = 1.0f) override {
            auto forward = glm::normalize(focal_point - position);
            position.x += scale * forward.x;
            position.z += scale * forward.z;
        }

        void move_backward(float scale = 1.0f) override {
            auto forward = glm::normalize(focal_point - position);
            position.x -= scale * forward.x;
            position.z -= scale * forward.z;
        }

        void move_left(float scale = 1.0f) override {
            auto forward = glm::normalize(focal_point - position);
            position -= scale * glm::cross(forward, up);
        }

        void move_right(float scale = 1.0f) override {
            auto forward = glm::normalize(focal_point - position);
            position += scale * glm::cross(forward, up);
        }

        void move_up(float scale = 1.0f) override {
            position += scale * up;
        }

        void move_down(float scale = 1.0f) override {
            position -= scale * up;
        }

        void pan_horizontal(float diff) override {
            horizontal_rotation += diff;
        }

        void pan_vertical(float diff) override {
            // TODO: make this do something
            vertical_rotation += diff;
        }
    protected:
        glm::vec3 up{0, 1, 0};
        float horizontal_rotation{0};
        float vertical_rotation{0};
    };
} // namespace engine


#endif //CIVILWAR_ORBITCAMERA_H
