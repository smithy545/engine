//
// Created by Philip Smith on 6/7/21.
//

#include <engine/InterfaceContainer.h>

#include <iostream>


namespace engine {
    InterfaceContainer::InterfaceContainer() = default;
    InterfaceContainer::InterfaceContainer(std::vector<InterfaceElement::Ptr>& initial_elements) {
        for(auto &element: initial_elements) {
            auto pos = element->get_center();
            auto key = point_key(pos.x, pos.y);
            if(m_elements.contains(key)) {
                auto& neighbor = m_elements[key];
                while(neighbor->get_next() != nullptr)
                    neighbor = neighbor->get_next();
                neighbor->set_next(element);
            } else {
                m_element_positions.emplace_back(pos.x, pos.y);
                m_elements[key] = element;
            }
        }
        m_collision_tree = std::make_shared<Quadtree>(m_element_positions);
    }

    glm::vec2 InterfaceContainer::get_center() {
        if(m_collision_tree == nullptr)
            return glm::vec2(0,0);
        auto bounds = m_collision_tree->bbox(m_collision_tree->root());
        return glm::vec2((bounds.ymin() + bounds.ymax())/2.0, (bounds.xmin() + bounds.xmax())/2.0);
    }

    InterfaceElement::Ptr InterfaceContainer::get_element_at(double x, double y) {
        if(m_collision_tree != nullptr
        && do_intersect(m_collision_tree->bbox(m_collision_tree->root()), Point_2{x,y})) {
            std::vector<Point_2> nearest(1);
            m_collision_tree->nearest_neighbors(Point_2{x, y}, 1, nearest.begin());
            return m_elements[point_key(nearest[0].x(), nearest[0].y())];
        }
        return nullptr;
    }

    bool InterfaceContainer::collides(double x, double y) {
        return get_element_at(x, y) != nullptr;
    }

    bool InterfaceContainer::handle_mouse_move(double x, double y) {
//        std::cout << "mouse move to " << x << " " << y << std::endl;
        auto triggered_element = get_element_at(x, y);
        if(triggered_element != nullptr)
            return triggered_element->handle_mouse_move(x, y);
        return true;
    }

    bool InterfaceContainer::handle_mouse_down(double x, double y, int button) {
//        std::cout << "mouse button " << button << " down at " << x << " " << y << std::endl;
        auto triggered_element = get_element_at(x, y);
        if(triggered_element != nullptr)
            return triggered_element->handle_mouse_down(x, y, button);
        return true;
    }

    bool InterfaceContainer::handle_mouse_up(double x, double y, int button) {
//        std::cout << "mouse button " << button << " up at " << x << " " << y << std::endl;
        auto triggered_element = get_element_at(x, y);
        if(triggered_element != nullptr)
            return triggered_element->handle_mouse_up(x, y, button);
        return true;
    }

    bool InterfaceContainer::handle_key_down(int code) {
//        std::cout << code << " down" << std::endl;
        return true;
    }

    bool InterfaceContainer::handle_key_up(int code) {
//        std::cout << code << " up" << std::endl;
        return true;
    }
} // namespace engine
