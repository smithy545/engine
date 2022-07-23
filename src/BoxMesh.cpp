/* Created by Philip Smith on 7/4/21.
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

#include <engine/render/mesh/BoxMesh.h>


namespace engine {
BoxMesh::BoxMesh(double x_offset,
                 double y_offset,
                 double z_offset,
                 double width,
                 double height,
                 double depth) {
    glm::vec3 anchor{x_offset, y_offset, z_offset};
    glm::vec3 w{width,0,0};
    glm::vec3 h{0,height,0};
    glm::vec3 d{0,0,depth};
    vertices = {
            anchor,
            anchor + glm::vec3{width, 0, 0},
            anchor + glm::vec3{width, height, 0},
            anchor + glm::vec3{0, height, 0},
            anchor + glm::vec3{0, 0, depth},
            anchor + glm::vec3{width, 0, depth},
            anchor + glm::vec3{width, height, depth},
            anchor + glm::vec3{0, height, depth}
    };
    // TODO: order indices properly
    indices = {
            0, 1, 2,
            2, 1, 3,
            0, 5, 4,
            5, 0, 1,
            0, 5, 7,
            0, 7, 3,
            3, 7, 6,
            6, 3, 2,
            1, 6, 5,
            6, 1, 3,
            4, 5, 6,
            6, 7, 4
    };
    // default color white
    for(auto v: vertices)
        colors.emplace_back(1,0, 0);
}

BoxMesh::BoxMesh(glm::vec3 offset, glm::vec3 scale)
: BoxMesh(offset.x, offset.y, offset.z, scale.x, scale.y, scale.z) {}
} // namespace engine
