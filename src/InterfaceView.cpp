//
// Created by Philip Smith on 6/7/21.
//

#include <engine/interface/InterfaceView.h>
#include <engine/interface/InterfaceController.h>
#include <engine/ManagedEntity.h>
#include <functional>
#include <utility>


namespace engine {
    InterfaceView::InterfaceView(InterfaceController& controller, entt::registry& registry)
    : IndependentEntity(registry), m_controller(controller) {}

    void InterfaceView::tick() {
        for(auto [pos, element]: m_elements) {
	        do {
		        if(auto* updatable = dynamic_cast<Tickable*>(element.get()))
		            updatable->tick();
		        element = element->get_next();
	        } while (element != nullptr);
        }
    }

    void InterfaceView::unload() {
        for(auto [pos, element]: m_elements) {
			do {
				if (auto *managed = dynamic_cast<ManagedEntity *>(element.get()))
					managed->deregister(registry);
				element = element->get_next();
			} while (element != nullptr);
        }
    }

    void InterfaceView::transition(InterfaceView::Ptr next_state) {
        unload();
        m_controller.set_state(std::move(next_state));
    }

    bool InterfaceView::collides(double x, double y) {
        auto element = get_nearest_element(x, y);
        if(element == nullptr)
            return false;
        auto* collidable = dynamic_cast<Collidable*>(element.get());
        return collidable != nullptr && collidable->collides(x, y);
    }

    glm::vec2 InterfaceView::get_center() {
    	auto& quadtree = m_element_finder->get_quadtree();
    	auto bounds = quadtree.bbox(quadtree.root());
        return {(bounds.ymin() + bounds.ymax())/2.0, (bounds.xmin() + bounds.xmax())/2.0};
    }

    InterfaceElement::Ptr InterfaceView::get_nearest_element(double x, double y) {
    	Point_2 p(x, y);
    	if(m_elements.contains(p))
    		return m_elements[p];
    	return m_elements[m_element_finder->find_closest(p)];
    }

    void InterfaceView::insert_element(InterfaceElement::Ptr element) {
        auto center = element->get_center();
        Point_2 key(center.x, center.y);
        if(m_elements.contains(key)) {
            auto itr = m_elements[key];
            while(itr->get_next() != nullptr)
                itr = itr->get_next();
            itr->set_next(element);
        } else
	        m_elements[key] = element;
		if(dynamic_cast<Collidable*>(element.get())) {
            std::vector<Point_2> positions;
			positions.push_back(key);
            for(const auto& pair: m_elements) {
	            if(dynamic_cast<Collidable*>(pair.second.get()))
		            positions.push_back(pair.first);
            }
            m_element_finder = std::make_shared<PointFinder>(positions);
        }
        if(auto* entity = dynamic_cast<ManagedEntity*>(element.get()))
            entity->register_with(registry);
        if(dynamic_cast<KeyEventSink *>(element.get()))
            on<KeyEvent>([element](KeyEvent& event, InterfaceView& emitter) {
                auto* handler = dynamic_cast<KeyEventSink*>(element.get());
                handler->handle(event, emitter);
            });
        if(dynamic_cast<MouseButtonEventSink *>(element.get()))
            on<MouseButtonEvent>([element](MouseButtonEvent& event, InterfaceView& emitter) {
                auto* handler = dynamic_cast<MouseButtonEventSink*>(element.get());
                handler->handle(event, emitter);
            });
        if(dynamic_cast<MouseMotionEventSink *>(element.get()))
            on<MouseMotionEvent>([element](MouseMotionEvent& event, InterfaceView& emitter) {
                auto* handler = dynamic_cast<MouseMotionEventSink*>(element.get());
                handler->handle(event, emitter);
            });
        if(dynamic_cast<MouseScrollEventSink *>(element.get()))
        	on<MouseScrollEvent>([element](MouseScrollEvent& event, InterfaceView& emitter) {
        		auto* handler = dynamic_cast<MouseScrollEventSink*>(element.get());
        		handler->handle(event, emitter);
        	});
        // TODO: store the above event connections to allow for later disconnection
    }

    void InterfaceView::remove_element(const InterfaceElement::Ptr& element) {
        auto center = element->get_center();
        Point_2 key(center.x, center.y);
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
            	std::vector<Point_2> positions;
            	for(const auto& pair: m_elements)
            		positions.push_back(pair.first);
            	m_element_finder = std::make_shared<PointFinder>(positions);
            }
        }
        if(auto* entity = dynamic_cast<ManagedEntity*>(element.get()))
            entity->deregister(registry);
    }

	entt::registry &InterfaceView::get_registry(){
    	return registry;
    }
} // namespace engine
