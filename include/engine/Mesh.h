//
// Created by Philip Smith on 5/13/21.
//

#ifndef CIVILWAR_MESH_H
#define CIVILWAR_MESH_H

#include <glm/glm.hpp>
#include <vector>


struct Mesh {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    std::vector<unsigned int> indices;
};

#endif //CIVILWAR_MESH_H
