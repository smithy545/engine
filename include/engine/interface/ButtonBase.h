//
// Created by Philip Smith on 7/15/21.
//

#ifndef ENGINE_BUTTONELEMENT_H
#define ENGINE_BUTTONELEMENT_H

#include "BoxElement.h"
#include "InterfaceView.h"

#include <engine/ManagedEntity.h>
#include <engine/input_events.h>
#include <engine/InstanceList.h>
#include <engine/sprite/ShapeSprite.h>


namespace engine {
    template<typename DownEvent, typename UpEvent>
    class ButtonBase : public BoxElement, public ManagedEntity, public MouseButtonEventSink {
    public:
        PTR(ButtonBase);

        ButtonBase(float x, float y, float width, float height) : BoxElement(x, y, width, height) {}

        entt::entity register_with(entt::registry& registry) override {
            ShapeSprite sprite;
            sprite.vertices.emplace_back(min.x, min.y);
            sprite.colors.emplace_back(0,1,1);

            sprite.vertices.emplace_back(min.x, max.y);
            sprite.colors.emplace_back(1,0,1);

            sprite.vertices.emplace_back(max.x, max.y);
            sprite.colors.emplace_back(1,1,0);

            sprite.vertices.emplace_back(max.x, min.y);
            sprite.colors.emplace_back(1,1,1);

            sprite.indices.push_back(0);
            sprite.indices.push_back(1);
            sprite.indices.push_back(2);
            sprite.indices.push_back(2);
            sprite.indices.push_back(3);
            sprite.indices.push_back(0);

            if(m_entity == entt::null)
                m_entity = registry.create();
            registry.emplace_or_replace<ShapeSprite>(m_entity, sprite);
            registry.patch<InstanceList>(m_entity, [](auto &instance_list) {
                instance_list.add_instance(glm::mat4(1));
            });
            return m_entity;
        }

        void handle(MouseButtonEvent& event, InterfaceView& emitter) override {
            if (m_down) {
                m_down = event.pressed;
                if(!m_down) {
                    auto* event_ptr = build_up_event(event, emitter);
                    emitter.publish<UpEvent*>(event_ptr);
                    delete event_ptr;
                }
            } else if(collides(event.x, event.y)) {
                m_down = event.pressed;
                if(m_down) {
                    auto* event_ptr = build_down_event(event, emitter);
                    emitter.publish<DownEvent*>(event_ptr);
                    delete event_ptr;
                }
            }
        }

        entt::entity get_entity() override {
            return m_entity;
        }

        virtual UpEvent* build_up_event(MouseButtonEvent &event, InterfaceView &emitter) = 0;

        virtual DownEvent* build_down_event(MouseButtonEvent &event, InterfaceView &emitter) = 0;
    protected:
        entt::entity m_entity{entt::null};
        bool m_down{false};
    };
} // namespace engine


#endif //ENGINE_BUTTONELEMENT_H
