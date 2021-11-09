//
// Created by Philip Smith on 5/25/21.
//

#ifndef ENGINE_SPRITE_H
#define ENGINE_SPRITE_H

#include <engine/VertexArrayObject.h>
#include <glm/glm.hpp>
#include <utility>
#include <vector>



namespace engine {
	struct Sprite : VertexArrayObject {
		Sprite() = default;

		Sprite(std::vector<glm::vec2> vertices, std::vector<unsigned int> indices) :
		vertices(std::move(vertices)), indices(std::move(indices)) {}

        std::vector<glm::vec2> vertices{};
        std::vector<unsigned int> indices{};
        bool visible{true};
    };
} // namespace engine

#endif //ENGINE_SPRITE_H
