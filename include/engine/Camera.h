//
// Created by Philip Smith on 5/18/21.
//

#ifndef CIVILWAR_CAMERA_H
#define CIVILWAR_CAMERA_H

#include <glm/glm.hpp>


struct Camera {
    glm::vec3 position{0, 0, 0};
    glm::vec3 forward{0, 0, 1};
    glm::vec3 up{0, 1, 0};
    float scale{1.0f};
};

#endif //CIVILWAR_CAMERA_H
