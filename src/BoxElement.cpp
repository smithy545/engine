//
// Created by Philip Smith on 7/15/21.
//

#define CUTE_C2_IMPLEMENTATION
#include <engine/BoxElement.h>


namespace engine {
    BoxElement::BoxElement(float x, float y, float width, float height)
            : c2AABB({x, y, x+width, y+height}), m_center((min.x+max.x)/2.0, (min.y+max.y)/2.0) {}

    glm::vec2 BoxElement::get_center() {
        return m_center;
    }

    bool BoxElement::collides(double x, double y) {
        return x >= min.x && y >= min.y && x < max.x && y < max.y;
    }
} // namespace engine