//
// Created by Philip Smith on 6/14/21.
//

#ifndef ENGINE_STEADICAM_H
#define ENGINE_STEADICAM_H

#include "Camera.h"


namespace engine {
    struct Steadicam : public Camera {
        PTR(Steadicam);

        glm::vec3 position;

		Steadicam(glm::vec3 position, glm::vec3 forward, glm::vec3 up);

        glm::vec3 get_mouse_world_coords(glm::vec2 pos) override;

        glm::vec3 get_forward_direction() const override;

        glm::vec3 get_position() const override;

        glm::mat4 get_view() const override;

        void pan_horizontal(float diff) override;

        void pan_vertical(float diff) override;

        void move_forward(float scale) override;

        void move_left(float scale) override;

        void move_up(float scale) override;
	protected:
	    glm::vec3 forward;
	    glm::vec3 up;
    };
} // namespace engine


#endif //ENGINE_STEADICAM_H
