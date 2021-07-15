//
// Created by Philip Smith on 6/7/21.
//

#include <engine/InterfaceView.h>

#include <functional>
#include <utility>
#include <engine/ManagedEntity.h>
#include <engine/InterfaceController.h>


namespace engine {
    InterfaceView::InterfaceView(InterfaceController& controller, entt::registry& registry)
    : IndependentEntity(registry), m_controller(controller) {}

    void InterfaceView::update(const RenderContext& context) {
        for(auto [pos, element]: m_elements) {
            if(auto* updatable = dynamic_cast<UpdateEntity*>(element.get()))
                updatable->update();
        }
    }

    void InterfaceView::unload() {
        for(auto [pos, element]: m_elements) {
            if(auto* managed = dynamic_cast<ManagedEntity*>(element.get()))
                registry.destroy(managed->get_entity());
        }
    }

    void InterfaceView::transition(InterfaceView::Ptr next_state) {
        unload();
        m_controller.set_state(std::move(next_state));
    }

    void InterfaceView::set_camera(Camera::Ptr camera) {
        m_controller.set_camera(camera);
    }

    bool InterfaceView::collides(double x, double y) {
        auto element = get_nearest_element(x, y);
        if(element == nullptr)
            return false;
        auto* collidable = dynamic_cast<Collidable*>(element.get());
        return collidable != nullptr && collidable->collides(x, y);
    }

    glm::vec2 InterfaceView::get_center() {
        if(m_collision_tree == nullptr)
            return glm::vec2(0,0);
        auto bounds = m_collision_tree->bbox(m_collision_tree->root());
        return glm::vec2((bounds.ymin() + bounds.ymax())/2.0, (bounds.xmin() + bounds.xmax())/2.0);
    }

    InterfaceElement::Ptr InterfaceView::get_nearest_element(double x, double y) {
        if(m_collision_tree != nullptr) {
            std::vector<Point_2> nearest(1);
            m_collision_tree->nearest_neighbors(Point_2{x, y}, 1, nearest.begin());
            return m_elements[point_key(nearest[0].x(), nearest[0].y())];
        }
        return nullptr;
    }

    void InterfaceView::insert_element(InterfaceElement::Ptr element) {
        auto center = element->get_center();
        auto key = point_key(center.x, center.y);
        if(m_elements.contains(key)) {
            auto itr = m_elements[key];
            while(itr->get_next() != nullptr)
                itr = itr->get_next();
            itr->set_next(element);
        } else if(dynamic_cast<Collidable*>(element.get()) != nullptr) {
            m_elements[key] = element;
            m_element_positions.emplace_back(center.x, center.y);
            m_collision_tree = std::make_shared<Quadtree>(m_element_positions);
            m_collision_tree->refine(QUADTREE_MAX_DEPTH, QUADTREE_BUCKET_SIZE);
        }
        if(auto* entity = dynamic_cast<ManagedEntity*>(element.get()))
            entity->register_with(registry);
        if(auto* handler = dynamic_cast<KeyEventSink*>(element.get()))
            on<KeyEvent>([element](KeyEvent& event, InterfaceView& emitter) {
                auto* handler = dynamic_cast<KeyEventSink*>(element.get());
                handler->handle(event, emitter);
            });
        if(auto* handler = dynamic_cast<MouseButtonEventSink*>(element.get()))
            on<MouseButtonEvent>([element](MouseButtonEvent& event, InterfaceView& emitter) {
                auto* handler = dynamic_cast<MouseButtonEventSink*>(element.get());
                handler->handle(event, emitter);
            });
        if(auto* handler = dynamic_cast<MouseMotionEventSink*>(element.get()))
            on<MouseMotionEvent>([element](MouseMotionEvent& event, InterfaceView& emitter) {
                auto* handler = dynamic_cast<MouseMotionEventSink*>(element.get());
                handler->handle(event, emitter);
            });
        // todo look into storing the above event connections to allow for later disconnection
    }

    void InterfaceView::remove_element(const InterfaceElement::Ptr& element) {
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
                for(int i = 0; i < m_element_positions.size(); i++) {
                    auto p = m_element_positions[i];
                    if(point_key(p.x(), p.y()) == key) {
                        m_element_positions.erase(m_element_positions.begin() + i);
                        break;
                    }
                }
                m_collision_tree = std::make_shared<Quadtree>(m_element_positions);
                m_collision_tree->refine(QUADTREE_MAX_DEPTH, QUADTREE_BUCKET_SIZE);
            }
        }
        if(auto* entity = dynamic_cast<ManagedEntity*>(element.get()))
            entity->deregister(registry);
    }
} // namespace engine
