//
// Created by Philip Smith on 7/4/21.
//

#ifndef ENGINE_BOXMESH_H
#define ENGINE_BOXMESH_H

#include "mesh.h"


namespace engine {
    struct BoxMesh : public Mesh {
        explicit BoxMesh(double x_offset = -0.5,
                double y_offset = -0.5,
                double z_offset = -0.5,
                double width = 1.0,
                double height = 1.0,
                double depth = 1.0);

        BoxMesh(glm::vec3 anchor, glm::vec3 scale);
    };
} // namespace engine


#endif //ENGINE_BOXMESH_H
