//
// Created by Philip Smith on 7/15/21.
//

#ifndef ENGINE_BOXELEMENT_H
#define ENGINE_BOXELEMENT_H

#include <cute_c2.h>
#include <engine/Collidable.h>
#include <glm/glm.hpp>
#include <utils/macros.h>

#include "InterfaceElement.h"


namespace engine {
    class BoxElement : public InterfaceElement, public Collidable, public c2AABB {
    public:
        PTR(BoxElement);

        BoxElement(float x, float y, float width, float height);

        glm::vec2 get_center() override;

        bool collides(double x, double y) override;

    private:
        glm::vec2 m_center;
    };
} // namespace engine


#endif //ENGINE_BOXELEMENT_H
