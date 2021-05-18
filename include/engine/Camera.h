//
// Created by Philip Smith on 5/18/21.
//

#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include <glm/glm.hpp>

namespace engine {
    struct Camera {
        glm::vec3 position{0, 0, 0};
        glm::vec3 forward{0, 0, 1};
        glm::vec3 up{0, 1, 0};
        float scale{1.0f};
    };
} // namespace engine

#endif //ENGINE_CAMERA_H
