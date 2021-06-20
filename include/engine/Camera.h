//
// Created by Philip Smith on 5/18/21.
//

#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include <glm/glm.hpp>
#include <utils/macros.h>


namespace engine {
    struct Camera {
        PTR(Camera);

        virtual glm::vec3 get_mouse_world_coords(glm::vec2 pos) = 0;
        virtual glm::vec3 get_forward_direction() const = 0;
        virtual glm::vec3 get_position() const = 0;
        virtual glm::mat4 get_view() const = 0;
        virtual void pan_horizontal(float diff) = 0;
        virtual void pan_vertical(float diff) = 0;
        virtual void move_forward(float scale = 1.0f) = 0;
        virtual void move_backward(float scale = 1.0f) = 0;
        virtual void move_left(float scale = 1.0f) = 0;
        virtual void move_right(float scale = 1.0f) = 0;
        virtual void move_up(float scale = 1.0f) = 0;
        virtual void move_down(float scale = 1.0f) = 0;
    };
} // namespace engine

#endif //ENGINE_CAMERA_H
