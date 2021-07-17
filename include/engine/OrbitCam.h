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

        OrbitCam(glm::vec3 position, glm::vec3 focal_point, bool filming = false);

        glm::vec3 get_mouse_world_coords(glm::vec2 pos) override;

        glm::vec3 get_forward_direction() const override;

        glm::vec3 get_position() const override;

        glm::mat4 get_view() const override;

        void move_toward(float scale = 1.0f);

        void move_away(float scale = 1.0f);

        void move_forward(float scale = 1.0f) override;

        void move_backward(float scale = 1.0f) override;

        void move_left(float scale = 1.0f) override;

        void move_right(float scale = 1.0f) override;

        void move_up(float scale = 1.0f) override;

        void move_down(float scale = 1.0f) override;

        void pan_horizontal(float diff) override;

        void pan_vertical(float diff) override;
    protected:
        glm::vec3 up{0, 1, 0};
        float vertical_rotation{0};
    };
} // namespace engine


#endif //ENGINE_ORBITCAMERA_H
