//
// Created by Philip Smith on 6/8/21.
//

#ifndef ENGINE_BOXELEMENT_H
#define ENGINE_BOXELEMENT_H

#include <cute_c2.h>
#include <glm/glm.hpp>
#include <string>
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

    template <typename DownEvent, typename UpEvent>
    class ButtonElement : public BoxElement, public ManagedEntity, public MouseButtonEventSink {
    public:
        PTR(ButtonElement);

        ButtonElement(float x, float y, float width, float height);

        entt::entity register_with(entt::registry& registry) override;

        void deregister(entt::registry& registry) override;

        entt::entity get_entity() override;

        void handle(MouseButtonEvent& event, InterfaceView& emitter) override;

        virtual UpEvent build_up_event(MouseButtonEvent& event, InterfaceView& emitter) = 0;

        virtual DownEvent build_down_event(MouseButtonEvent& event, InterfaceView& emitter) = 0;
    protected:
        entt::entity m_entity{entt::null};
        bool down{false};
    };

    template <typename DownEvent, typename UpEvent>
    class TexturedButton : public ButtonElement<DownEvent, UpEvent> {
    public:
        PTR(TexturedButton);

        TexturedButton(std::string up_texture, std::string down_texture, float x, float y, float width, float height);

        entt::entity register_with(entt::registry& registry) override;
    protected:
        std::string unclicked_tex_name;
        std::string clicked_tex_name;
    };

    class StartButton : public TexturedButton<TexSwapEvent, StartEvent> {
    public:
        PTR(StartButton);

        explicit StartButton(const RenderContext& context);

        StartEvent build_up_event(MouseButtonEvent& event, InterfaceView& emitter) override;

        TexSwapEvent build_down_event(MouseButtonEvent& event, InterfaceView& emitter) override;
    };

    class ExitButton : public TexturedButton<TexSwapEvent, ExitEvent> {
    public:
        PTR(ExitButton);

        explicit ExitButton(const RenderContext& context);

        ExitEvent build_up_event(MouseButtonEvent& event, InterfaceView& emitter) override;

        TexSwapEvent build_down_event(MouseButtonEvent& event, InterfaceView& emitter) override;
    };
} // namespace engine


#endif //ENGINE_BOXELEMENT_H
