//
// Created by Philip Smith on 10/10/21.
//

#ifndef ENGINE_TEXTURESPRITE_H
#define ENGINE_TEXTURESPRITE_H

#include "Sprite.h"


namespace engine {
	struct TextureSprite : public Sprite {
		std::string name;
		std::vector<glm::vec2> uvs;
	};
} // namespace engine

#endif //ENGINE_TEXTURESPRITE_H
