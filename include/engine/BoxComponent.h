//
// Created by Philip Smith on 7/15/21.
//

#ifndef ENGINE_BOXELEMENT_H
#define ENGINE_BOXELEMENT_H

#include <cute_c2.h>
#include <glm/glm.hpp>
#include <utils/macros.h>

#include "physics.h"


namespace engine {
    class BoxComponent : public Collidable, public c2AABB {
    public:
        PTR(BoxComponent);

        BoxComponent(float x, float y, float width, float height);

        bool collides(double x, double y) override;
    };
} // namespace engine


#endif //ENGINE_BOXELEMENT_H
