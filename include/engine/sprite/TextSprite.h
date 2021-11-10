//
// Created by Philip Smith on 10/13/21.
//

#ifndef ENGINE_TEXTSPRITE_H
#define ENGINE_TEXTSPRITE_H

#include <engine/VertexArrayObject.h>
#include <glm/glm.hpp>
#include <string>
#include <utility>


namespace engine {
	struct TextSprite : VertexArrayObject {
		TextSprite() = default;

		TextSprite(std::string font, std::string text, float x, float y, float scale, glm::vec3 color)
		: font(std::move(font)), text(std::move(text)), x(x), y(y), scale(scale), color(color) {}

		std::string font;
		std::string text;
		float x{0}, y{0};
		float scale{1};
		glm::vec3 color{1,1,1};
		bool visible{true};
	};
} // namespace

#endif //ENGINE_TEXTSPRITE_H
