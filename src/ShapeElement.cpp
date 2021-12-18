//
// Created by Philip Smith on 12/14/21.
//

#include <engine/interface/ShapeElement.h>


namespace engine {
	ShapeElement::ShapeElement(float x, float y, const std::vector<glm::vec2>& vertices)
	: c2Poly(), m_center(x, y) {
		assert(vertices.size() > 2);
		assert(vertices.size() <= C2_MAX_POLYGON_VERTS);
		for(auto i = 0; i < vertices.size(); i++)
			verts[i] = {x + vertices[i].x, y + vertices[i].y};
		count = vertices.size();
		c2MakePoly(this);
	}

	glm::vec2 ShapeElement::get_center() {
		return m_center;
	}

	bool ShapeElement::collides(double x, double y) {
		return c2CircletoPoly({{static_cast<float>(x), static_cast<float>(y)}, 1}, this, nullptr);
	}
} // namespace engine
