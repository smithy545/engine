/* Created by Philip Smith on 6/14/21.
MIT License

Copyright (c) 2021 Philip Arturo Smith

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef ENGINE_ORBITCAM_H
#define ENGINE_ORBITCAM_H

#include "Camera.h"


namespace engine::render {

struct OrbitCam : public Camera {
	USEPTR(OrbitCam);

	glm::vec3 position;
	glm::vec3 focal_point;

	OrbitCam(glm::vec3 position, glm::vec3 focal_point);

	glm::vec3 get_mouse_world_coords(glm::vec2 pos) override;

    glm::vec3 get_forward_direction() const override;

    glm::vec3 get_position() const override;

    glm::mat4 get_view() const override;

    void move_toward(float scale);

    void pan_forward(float scale) override;

    void pan_left(float scale) override;

	void pan_up(float scale) override;

	void turn_horizontal(float diff) override;

	void turn_vertical(float diff) override;

protected:
	glm::vec3 up{0, 0, 1};
	float vertical_rotation{0};
};

} // namespace engine::render


#endif //ENGINE_ORBITCAM_H
