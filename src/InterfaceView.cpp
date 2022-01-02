//
// Created by Philip Smith on 6/7/21.
//

#include <engine/physics.h>
#include <engine/interface/InterfaceView.h>
#include <engine/interface/InterfaceController.h>
#include <memory>
#include <utility>


namespace engine {
    InterfaceView::InterfaceView(InterfaceController& controller, entt::registry& registry)
    : IndependentEntity(registry), m_controller(controller) {}

    void InterfaceView::tick() {
		auto view = registry.view<Tickable*>();
        for(auto entity: view) {
			auto [ticker] = view.get(entity);
	        ticker->tick();
        }
    }

	void InterfaceView::load(const RenderContext& context) {
		on<MouseButtonEvent>([&](MouseButtonEvent& event, InterfaceView& emitter) {
			mouse_button_callback(event, emitter);
		});
		on<MouseMotionEvent>([&](MouseMotionEvent& event, InterfaceView& emitter) {
			mouse_motion_callback(event, emitter);
		});
	}

    void InterfaceView::unload() {
        for(auto [key, element]: m_entity_by_position) {
			if(auto managed = dynamic_cast<ManagedEntity*>(element.get()))
				managed->deregister(registry);
        }
		// clear event listeners
		clear();
    }

	void InterfaceView::mouse_button_callback(MouseButtonEvent& event, InterfaceView& emitter) {
		if(auto element = get_nearest(event.x, event.y)) {
			if (auto* handler = dynamic_cast<MouseButtonEventSink*>(element.get())) {
				if (auto* collidable = dynamic_cast<Collidable*>(element.get())) {
					if (collidable->collides(event.x, event.y))
						handler->handle(event, emitter);
				} else
					handler->handle(event, emitter);
			}
		}
	}

	void InterfaceView::mouse_motion_callback(MouseMotionEvent& event, InterfaceView& emitter) {
		// TODO: make "focused" entity base class that takes over mouse motion when focused
		if(auto element = get_nearest(event.x, event.y)) {
			if (auto* handler = dynamic_cast<MouseMotionEventSink*>(element.get())) {
				if (auto *collidable = dynamic_cast<Collidable *>(element.get())) {
					if (collidable->collides(event.x, event.y))
						handler->handle(event, emitter);
				} else
					handler->handle(event, emitter);
			}
		}
	}

	void InterfaceView::transition(InterfaceView::Ptr next_state) {
        unload();
        m_controller.set_state(std::move(next_state));
    }

    InterfaceEntity::Ptr InterfaceView::get_nearest(double x, double y) {
    	Point_2 p(x, y);
		if(m_entity_by_position.contains(p))
			return m_entity_by_position.at(p);
		if(m_nearest_entity_finder != nullptr) {
			p = m_nearest_entity_finder->find_closest(p);
			return m_entity_by_position.at(p);
		}
	    return nullptr;
	}

	entt::entity InterfaceView::insert_element(const InterfaceEntity::Ptr& element) {
	    if(m_entity_by_position.contains(element->pos)) {
			auto itr = m_entity_by_position.at(element->pos);
			while(itr->next != nullptr)
				itr = itr->next;
			itr->next = element;
	    } else {
		    // rebuild point finder upon position insertion
		    m_entity_by_position.insert({element->pos, element});
		    std::vector<Point_2> positions;
		    positions.push_back(element->pos);
		    for (const auto &pair: m_entity_by_position)
				positions.push_back(pair.first);
		    m_nearest_entity_finder = std::make_shared<PointFinder>(positions);
		}
		element->register_with(registry);
	    if(auto* handler = dynamic_cast<KeyEventSink*>(element.get()))
		    on<KeyEvent>([handler](KeyEvent& event, InterfaceView& emitter) {
			    handler->handle(event, emitter);
		    });
	    if(auto* handler = dynamic_cast<MouseScrollEventSink *>(element.get()))
		    on<MouseScrollEvent>([handler](MouseScrollEvent& event, InterfaceView& emitter) {
			    handler->handle(event, emitter);
		    });
        // TODO: store the above event connections to allow for later disconnection
		return element->get_entity();
    }

    void InterfaceView::remove_element(const InterfaceEntity::Ptr& element) {
	    auto key = element->pos;
        if(m_entity_by_position.contains(key)) {
            if(m_entity_by_position[key] == element) {
                if(element->next == nullptr)
	                m_entity_by_position.erase(key);
                else
	                m_entity_by_position[key] = element->next;
            } else {
                auto itr = m_entity_by_position[key];
                while(itr != nullptr && itr->next != element)
                    itr = itr->next;
                if(itr != nullptr)
                    itr->next = element->next;
            }
            if(!m_entity_by_position.contains(key)) {
				// rebuild point finder upon position removal
            	std::vector<Point_2> positions;
            	for(const auto& pair: m_entity_by_position)
            		positions.push_back(pair.first);
	            m_nearest_entity_finder = std::make_shared<PointFinder>(positions);
            }
        }
        if(auto* e = dynamic_cast<ManagedEntity*>(element.get()))
            e->deregister(registry);
    }

	entt::registry& InterfaceView::get_registry() {
    	return registry;
    }
} // namespace engine
