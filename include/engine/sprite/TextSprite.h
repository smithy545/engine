//
// Created by Philip Smith on 10/13/21.
//

#ifndef ENGINE_TEXTSPRITE_H
#define ENGINE_TEXTSPRITE_H

#include <glm/glm.hpp>
#include <string>


namespace engine {
	struct TextSprite {
		std::string font;
		std::string text;
		float x, y;
		float scale;
		glm::vec3 color;
		bool visible{true};
	};
} // namespace

#endif //ENGINE_TEXTSPRITE_H
