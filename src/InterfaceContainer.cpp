//
// Created by Philip Smith on 6/7/21.
//

#include <engine/InterfaceContainer.h>

#include <functional>
#include <engine/ManagedEntity.h>


namespace engine {
    InterfaceContainer::InterfaceContainer(entt::registry& registry) : IndependentEntity(registry) {}

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
        if(element == nullptr)
            return false;
        auto* collideable = dynamic_cast<Collideable*>(element.get());
        return collideable != nullptr && collideable->collides(x, y);
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
        if(auto* entity = dynamic_cast<ManagedEntity*>(element.get()))
            entity->register_with(registry);
        if(auto* handler = dynamic_cast<KeyEventSink*>(element.get()))
            on<KeyEvent>([handler](KeyEvent& event, InterfaceContainer& emitter) {
                handler->handle(event, emitter);
            });
        if(auto* handler = dynamic_cast<MouseButtonEventSink*>(element.get()))
            on<MouseButtonEvent>([handler](MouseButtonEvent& event, InterfaceContainer& emitter) {
                handler->handle(event, emitter);
            });
        if(auto* handler = dynamic_cast<MouseMotionEventSink*>(element.get()))
            on<MouseMotionEvent>([handler](MouseMotionEvent& event, InterfaceContainer& emitter) {
                handler->handle(event, emitter);
            });
        // todo look into storing the above connections to allow for later disconnection
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
        if(auto* entity = dynamic_cast<ManagedEntity*>(element.get()))
            entity->deregister(registry);
    }

    void InterfaceContainer::clear() {
        m_collision_tree = nullptr;
        m_element_positions.clear();
        m_elements.clear();
    }
} // namespace engine
