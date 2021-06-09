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
        m_collision_tree->refine(4, 10);
    }

    void InterfaceContainer::register_with(entt::registry& registry) {
        for(auto [key, element]: m_elements)
            element->register_with(registry);
    }

    void InterfaceContainer::deregister(entt::registry &registry) {
        for(auto [key, element]: m_elements)
            element->deregister(registry);
    }

    InterfaceElement::Ptr InterfaceContainer::get_nearest_element(double x, double y) {
        if(m_collision_tree != nullptr) {
            std::vector<Point_2> nearest(1);
            m_collision_tree->nearest_neighbors(Point_2{x, y}, 1, nearest.begin());
            return m_elements[point_key(nearest[0].x(), nearest[0].y())];
        }
        return nullptr;
    }

    bool InterfaceContainer::collides(double x, double y) {
        auto element = get_nearest_element(x, y);
        return element != nullptr && element->collides(x, y);
    }

    bool InterfaceContainer::handle_mouse_move(double x, double y) {
//        std::cout << "mouse move to " << x << " " << y << std::endl;
        auto element = get_nearest_element(x, y);
        if(element != nullptr && element->collides(x, y))
            return element->handle_mouse_move(x, y);
        return true;
    }

    bool InterfaceContainer::handle_mouse_down(double x, double y, int button) {
//        std::cout << "mouse button " << button << " down at " << x << " " << y << std::endl;
        auto element = get_nearest_element(x, y);
        if(element != nullptr && element->collides(x, y))
            return element->handle_mouse_down(x, y, button);
        return true;
    }

    bool InterfaceContainer::handle_mouse_up(double x, double y, int button) {
//        std::cout << "mouse button " << button << " up at " << x << " " << y << std::endl;
        auto element = get_nearest_element(x, y);
        if(element != nullptr && element->collides(x, y))
            return element->handle_mouse_up(x, y, button);
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

    glm::vec2 InterfaceContainer::get_center() {
        if(m_collision_tree == nullptr)
            return glm::vec2(0,0);
        auto bounds = m_collision_tree->bbox(m_collision_tree->root());
        return glm::vec2((bounds.ymin() + bounds.ymax())/2.0, (bounds.xmin() + bounds.xmax())/2.0);
    }

    void InterfaceContainer::insert_element(InterfaceElement::Ptr element) {
        auto center = element->get_center();
        auto key = point_key(center.x, center.y);
        if(m_elements.contains(key)) {
            auto itr = m_elements[key];
            while(itr->get_next() != nullptr)
                itr = itr->get_next();
            itr->set_next(element);
        } else {
            m_elements[key] = element;
            m_element_positions.emplace_back(center.x, center.y);
            m_collision_tree = std::make_shared<Quadtree>(m_element_positions);
            m_collision_tree->refine(4, 10);
        }
    }

    void InterfaceContainer::remove_element(const InterfaceElement::Ptr& element) {
        auto center = element->get_center();
        auto key = point_key(center.x, center.y);
        if(m_elements.contains(key)) {
            if(m_elements[key] == element) {
                if(element->get_next() == nullptr)
                    m_elements.erase(key);
                else
                    m_elements[key] = element->get_next();
            } else {
                auto itr = m_elements[key];
                while(itr != nullptr && itr->get_next() != element)
                    itr = itr->get_next();
                if(itr != nullptr) {
                    auto next = element->get_next();
                    itr->set_next(next);
                }
            }
            if(!m_elements.contains(key)) {
                m_collision_tree = std::make_shared<Quadtree>(m_element_positions);
                m_collision_tree->refine(4, 10);
            }
        }
    }

    void InterfaceContainer::clear() {
        m_collision_tree = nullptr;
        m_element_positions.clear();
        m_elements.clear();
    }
} // namespace engine
