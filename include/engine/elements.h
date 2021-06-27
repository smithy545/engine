//
// Created by Philip Smith on 6/8/21.
//

#ifndef ENGINE_BOXELEMENT_H
#define ENGINE_BOXELEMENT_H

#include <cute_c2.h>
#include <glm/glm.hpp>
#include <utils/macros.h>

#include "Collidable.h"
#include "input_events.h"
#include "game_events.h"
#include "InterfaceElement.h"
#include "ManagedEntity.h"
#include "RenderContext.h"


namespace engine {
    class BoxElement : public InterfaceElement, public Collidable, public c2AABB {
    public:
        PTR(BoxElement);

        BoxElement(float x, float y, float width, float height);

        glm::vec2 get_center() override;

        bool collides(double x, double y) override;
    private:
        glm::vec2 m_center;
    };

    template <typename Event>
    class ButtonElement : public BoxElement, public ManagedEntity, public MouseButtonEventSink {
    public:
        PTR(ButtonElement);

        ButtonElement(float x, float y, float width, float height);

        entt::entity register_with(entt::registry& registry) override;

        void deregister(entt::registry& registry) override;

        entt::entity get_entity() override;

        void handle(MouseButtonEvent& event, InterfaceContainer& emitter) override;

        virtual Event build_event(MouseButtonEvent& event, InterfaceContainer& emitter) = 0;
    protected:
        entt::entity m_entity{entt::null};
    };

    class StartButton : public ButtonElement<StartEvent> {
    public:
        PTR(StartButton);

        explicit StartButton(const RenderContext& context);

        entt::entity register_with(entt::registry& registry) override;

        StartEvent build_event(MouseButtonEvent& event, InterfaceContainer& emitter) override;
    };

    class ExitButton : public ButtonElement<ExitEvent> {
    public:
        PTR(ExitButton);

        explicit ExitButton(const RenderContext& context);

        ExitEvent build_event(MouseButtonEvent& event, InterfaceContainer& emitter) override;
    };
} // namespace engine


#endif //ENGINE_BOXELEMENT_H
