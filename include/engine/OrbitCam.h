//
// Created by Philip Smith on 6/14/21.
//

#ifndef ENGINE_ORBITCAMERA_H
#define ENGINE_ORBITCAMERA_H

#include "Camera.h"


namespace engine {
    struct OrbitCam : public Camera {
        PTR(OrbitCam);

        glm::vec3 position;
        glm::vec3 focal_point;

        OrbitCam(glm::vec3 position, glm::vec3 focal_point);

        glm::vec3 get_mouse_world_coords(glm::vec2 pos) override;

        glm::vec3 get_forward_direction() const override;

        glm::vec3 get_position() const override;

        glm::mat4 get_view() const override;

        void move_toward(float scale);

        void move_forward(float scale) override;

        void move_left(float scale) override;

        void move_up(float scale) override;

        void pan_horizontal(float diff) override;

        void pan_vertical(float diff) override;
    protected:
        glm::vec3 up{0, 1, 0};
        float vertical_rotation{0};
    };
} // namespace engine


#endif //ENGINE_ORBITCAMERA_H
