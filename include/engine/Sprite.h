//
// Created by Philip Smith on 5/25/21.
//

#ifndef ENGINE_SPRITE_H
#define ENGINE_SPRITE_H

#include <glm/glm.hpp>
#include <vector>


namespace engine {
    struct Sprite {
        std::vector<glm::vec2> vertices;
        std::vector<glm::vec3> colors;
        std::vector<unsigned int> indices;
    };
} // namespace engine

#endif //ENGINE_SPRITE_H
