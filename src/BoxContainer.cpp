//
// Created by Philip Smith on 11/10/21.
//

#include <engine/interface/BoxContainer.h>


namespace engine {
	BoxContainer::BoxContainer(float x, float y, float width, float height, float padding, glm::vec3 color)
	: BoxElement(x, y, width, height), m_padding(padding), m_color(color) {}
} // namespace engine