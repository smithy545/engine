/* Created by Philip Smith on 5/18/21.
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

#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include <glm/glm.hpp>
#include <utils/macros.h>


namespace engine::render {

struct Camera {
	USEPTR(Camera);

	virtual glm::vec3 get_mouse_world_coords(glm::vec2 pos) = 0;

	virtual glm::vec3 get_forward_direction() const = 0;

	virtual glm::vec3 get_position() const = 0;

	virtual glm::mat4 get_view() const = 0;

	virtual void turn_horizontal(float diff) = 0;

	virtual void turn_vertical(float diff) = 0;

	virtual void pan_forward(float scale) = 0;

	inline void pan_backward(float scale) {
		pan_forward(-scale);
	}

	virtual void pan_left(float scale) = 0;

	inline void pan_right(float scale) {
		pan_left(-scale);
	}

	virtual void pan_up(float scale) = 0;

	inline void pan_down(float scale) {
		pan_up(-scale);
	}
};

} // namespace engine::render

#endif //ENGINE_CAMERA_H
