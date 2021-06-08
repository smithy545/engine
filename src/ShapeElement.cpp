//
// Created by Philip Smith on 6/8/21.
//

#include <engine/ShapeElement.h>
#include <iostream>


namespace engine {
    ShapeElement::ShapeElement(glm::vec2 center) : m_center(center) {}

    bool ShapeElement::handle_mouse_move(double x, double y) {
        return false;
    }

    bool ShapeElement::handle_mouse_down(double x, double y, int button) {
        return false;
    }

    bool ShapeElement::handle_mouse_up(double x, double y, int button) {
        return false;
    }

    bool ShapeElement::handle_key_down(int code) {
        return false;
    }

    bool ShapeElement::handle_key_up(int code) {
        return false;
    }

    glm::vec2 ShapeElement::get_center() {
        return m_center;
    }

    bool ShapeElement::collides(double x, double y) {
        return false;
    }
} // namespace engine