//
// Created by Philip Smith on 7/15/21.
//

#define CUTE_C2_IMPLEMENTATION
#include <engine/BoxComponent.h>


namespace engine {
    BoxComponent::BoxComponent(float x, float y, float width, float height)
	: c2AABB({x, y, x+width, y+height}) {}

    bool BoxComponent::collides(double x, double y) {
		// avoid any possible c2AABB overhead for point collisions
        return x >= min.x && y >= min.y && x < max.x && y < max.y;
    }
} // namespace engine
