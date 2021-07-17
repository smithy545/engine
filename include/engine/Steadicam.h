//
// Created by Philip Smith on 6/14/21.
//

#ifndef ENGINE_STEADICAM_H
#define ENGINE_STEADICAM_H

#include "Camera.h"


namespace engine {
    struct Steadicam : public Camera {
        glm::vec3 position;
        bool filming;

        Steadicam(glm::vec3 position, bool filming = false);

        glm::vec3 get_mouse_world_coords(glm::vec2 pos) override;

        glm::vec3 get_forward_direction() const override;

        glm::vec3 get_position() const override;

        glm::mat4 get_view() const override;

        void pan_horizontal(float diff) override;

        void pan_vertical(float diff) override;

        void move_forward(float scale = 1.0f);

        void move_backward(float scale = 1.0f);

        void move_left(float scale = 1.0f) override;

        void move_right(float scale = 1.0f) override;

        void move_up(float scale = 1.0f) override;

        void move_down(float scale = 1.0f) override;
    protected:
        glm::vec3 forward{0, 0, 1};
        glm::vec3 up{0, 1, 0};
    };
} // namespace engine


#endif //ENGINE_STEADICAM_H
