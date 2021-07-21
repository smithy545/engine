//
// Created by Philip Smith on 7/4/21.
//

#include <engine/BoxMesh.h>


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

    BoxMesh::BoxMesh(glm::vec3 anchor, glm::vec3 scale)
    : BoxMesh(anchor.x, anchor.y, anchor.z, scale.x, scale.y, scale.z) {}
} // namespace engine
