//
// Created by Philip Smith on 6/8/21.
//

#ifndef ENGINE_BOXELEMENT_H
#define ENGINE_BOXELEMENT_H

#include <cute_c2.h>
#include <glm/glm.hpp>
#include <utils/macros.h>

#include "InterfaceElement.h"


namespace engine {
    class BoxElement : public InterfaceElement, public c2AABB {
    public:
        PTR(BoxElement);

        BoxElement(float x, float y, float width, float height);

        glm::vec2 get_center() override;

        bool collides(double x, double y) override;
    private:
        glm::vec2 m_center;
    };

    class ColoredBoxElement : public BoxElement {
    public:
        PTR(ColoredBoxElement);

        ColoredBoxElement(float x, float y, float width, float height, glm::vec3 color);

        void register_with(entt::registry& registry) override;

        void deregister(entt::registry& registry) override;

        bool handle_mouse_move(double x, double y) override;

        bool handle_mouse_down(double x, double y, int button) override;

        bool handle_mouse_up(double x, double y, int button) override;

        bool handle_key_down(int code) override;

        bool handle_key_up(int code) override;
    private:
        entt::entity entity{entt::null};
    VAR(glm::vec3, color, public, public);
    };
} // namespace engine


#endif //ENGINE_BOXELEMENT_H
