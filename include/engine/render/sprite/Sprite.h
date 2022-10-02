/* Created by Philip Smith on 5/25/21.
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

#ifndef ENGINE_SPRITE_H
#define ENGINE_SPRITE_H

#include <engine/render/VertexArrayObject.h>
#include <glm/glm.hpp>
#include <utility>
#include <vector>


namespace engine::render {

struct Sprite : VertexArrayObject {
	Sprite() = default;

	Sprite(std::vector<glm::vec2> vertices, std::vector<unsigned int> indices) :
	vertices(std::move(vertices)), indices(std::move(indices)) {}

    std::vector<glm::vec2> vertices{};
    std::vector<unsigned int> indices{};
};

} // namespace engine::render

#endif //ENGINE_SPRITE_H
