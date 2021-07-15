//
// Created by Philip Smith on 6/14/21.
//

#ifndef ENGINE_ORBITCAMERA_H
#define ENGINE_ORBITCAMERA_H

#include "Camera.h"
#include <glm/gtx/rotate_vector.hpp>


namespace engine {
    struct OrbitCam : public Camera {
        glm::vec3 position;
        glm::vec3 focal_point;
        bool filming;

        OrbitCam(glm::vec3 position, glm::vec3 focal_point, bool filming = false)
        : position(position), focal_point(focal_point), filming(filming) {}

        glm::vec3 get_mouse_world_coords(glm::vec2 pos) override {
            auto forward = get_forward_direction();
            auto right = glm::normalize(glm::cross(forward, up));
            return position + right*pos.x + glm::cross(right, forward)*pos.y;
        }

        glm::vec3 get_forward_direction() const override {
            auto forward = glm::normalize(focal_point - position);
            return glm::rotate(forward, horizontal_rotation, up);
        }

        glm::vec3 get_position() const override {
            return position;
        }

        glm::mat4 get_view() const override {
            if(horizontal_rotation == 0.0f)
                return  glm::lookAt(position, focal_point, up);
            return  glm::lookAt(position, position + get_forward_direction(), up);
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


#endif //ENGINE_ORBITCAMERA_H
