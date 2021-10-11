//
// Created by Philip Smith on 10/10/21.
//

#ifndef ENGINE_SHAPESPRITE_H
#define ENGINE_SHAPESPRITE_H

#include "Sprite.h"


namespace engine {
	struct ShapeSprite : public Sprite {
		std::vector<glm::vec3> colors;
	};
} // namespace engine


#endif //ENGINE_SHAPESPRITE_H
