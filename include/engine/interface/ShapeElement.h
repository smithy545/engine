//
// Created by Philip Smith on 12/14/21.
//

#ifndef ENGINE_SHAPEELEMENT_H
#define ENGINE_SHAPEELEMENT_H

#include <cute_c2.h>
#include <engine/Collidable.h>
#include <glm/glm.hpp>
#include <utils/macros.h>
#include <vector>

#include "InterfaceElement.h"


namespace engine {
	class ShapeElement : public InterfaceElement, public Collidable, public c2Poly {
	public:
		PTR(ShapeElement);

		ShapeElement(float x, float y, const std::vector<glm::vec2>& vertices);

		glm::vec2 get_center() override;

		bool collides(double x, double y) override;

	private:
		glm::vec2 m_center;
	};
} // namespace engine


#endif //ENGINE_SHAPEELEMENT_H
