//
// Created by Philip Smith on 5/18/21.
//

#ifndef CIVILWAR_INSTANCELIST_H
#define CIVILWAR_INSTANCELIST_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>


struct InstanceList {
    GLuint id;
    std::vector<glm::mat4> models;
};

#endif //CIVILWAR_INSTANCELIST_H
