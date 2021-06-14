//
// Created by Philip Smith on 6/8/21.
//

#ifndef ENGINE_BOXELEMENT_H
#define ENGINE_BOXELEMENT_H

#include <cute_c2.h>
#include <glm/glm.hpp>
#include <utils/macros.h>

#include "Collideable.h"
#include "InterfaceElement.h"


namespace engine {
    class BoxElement : public InterfaceElement, public c2AABB, public Collideable {
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
