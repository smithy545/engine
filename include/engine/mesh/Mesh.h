//
// Created by Philip Smith on 5/13/21.
//

#ifndef ENGINE_MESH_H
#define ENGINE_MESH_H

#include <engine/VertexArrayObject.h>
#include <glm/glm.hpp>
#include <utility>
#include <vector>


namespace engine {
	struct Mesh : VertexArrayObject {
		Mesh() = default;

		Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec3> colors, std::vector<unsigned int> indices) :
		vertices(std::move(vertices)), colors(std::move(colors)), indices(std::move(indices)) {}

        std::vector<glm::vec3> vertices{};
        std::vector<glm::vec3> colors{};
        std::vector<unsigned int> indices{};
        bool visible{true};
    };
} // namespace engine

#endif //ENGINE_MESH_H
