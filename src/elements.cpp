//
// Created by Philip Smith on 6/8/21.
//
#define CUTE_C2_IMPLEMENTATION
#include <engine/elements.h>

#include <engine/InstanceList.h>
#include <engine/InterfaceContainer.h>
#include <engine/Sprite.h>


namespace engine {
    BoxElement::BoxElement(float x, float y, float width, float height)
    : c2AABB({x, y, x+width, y+height}), m_center((min.x+max.x)/2.0, (min.y+max.y)/2.0) {}

    glm::vec2 BoxElement::get_center() {
        return m_center;
    }

    bool BoxElement::collides(double x, double y) {
        return x >= min.x && y >= min.y && x < max.x && y < max.y;
    }

    template <typename Event>
    ButtonElement<Event>::ButtonElement(float x, float y, float width, float height) : BoxElement(x, y, width, height) {}

    template <typename Event>
    entt::entity ButtonElement<Event>::register_with(entt::registry& registry) {
        Sprite sprite;
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
        registry.emplace_or_replace<Sprite>(m_entity, sprite);
        registry.patch<InstanceList>(m_entity, [](auto &instance_list) {
            instance_list.add_instance(glm::mat4(1));
        });
        return entt::null;
    }

    template <typename Event>
    void ButtonElement<Event>::deregister(entt::registry& registry) {
        registry.destroy(m_entity);
    }

    template <typename Event>
    void ButtonElement<Event>::handle(MouseButtonEvent& event, InterfaceContainer& emitter) {
        if(collides(event.x, event.y) && !event.pressed) // trigger on mouse up
            emitter.publish<Event>(build_event(event, emitter));
    }

    template<typename Event>
    entt::entity ButtonElement<Event>::get_entity() {
        return m_entity;
    }

    StartButton::StartButton(const RenderContext& ctx) :
    ButtonElement<StartEvent>(ctx.screen_width/2 - 50, 20, 100, 50) {}

    StartEvent StartButton::build_event(MouseButtonEvent& event, InterfaceContainer& emitter) {
        return StartEvent{};
    }

    ExitButton::ExitButton(const RenderContext& ctx)
    : ButtonElement<ExitEvent>(ctx.screen_width/2 - 50, 100, 100, 50) {}

    ExitEvent ExitButton::build_event(MouseButtonEvent& event, InterfaceContainer& emitter) {
        return ExitEvent{};
    }
} // namespace engine
