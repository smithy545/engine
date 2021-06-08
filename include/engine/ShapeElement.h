//
// Created by Philip Smith on 6/8/21.
//

#ifndef ENGINE_SHAPEELEMENT_H
#define ENGINE_SHAPEELEMENT_H

#include "InterfaceElement.h"


namespace engine {
    class ShapeElement : public InterfaceElement {
    public:
        PTR(ShapeElement);

        explicit ShapeElement(glm::vec2 center);

        bool handle_mouse_move(double x, double y) override;
        bool handle_mouse_down(double x, double y, int button) override;
        bool handle_mouse_up(double x, double y, int button) override;
        bool handle_key_down(int code) override;
        bool handle_key_up(int code) override;
        glm::vec2 get_center() override;
        bool collides(double x, double y) override;
    VAR_SET(glm::vec2, center, protected);
    };
} // namespace engine


#endif //ENGINE_SHAPEELEMENT_H
