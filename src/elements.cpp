//
// Created by Philip Smith on 6/8/21.
//
#define CUTE_C2_IMPLEMENTATION
#include <engine/elements.h>

#include <engine/InstanceList.h>
#include <engine/InterfaceView.h>
#include <engine/sprites.h>

#include <utility>


namespace engine {
    BoxElement::BoxElement(float x, float y, float width, float height)
    : c2AABB({x, y, x+width, y+height}), m_center((min.x+max.x)/2.0, (min.y+max.y)/2.0) {}

    glm::vec2 BoxElement::get_center() {
        return m_center;
    }

    bool BoxElement::collides(double x, double y) {
        return x >= min.x && y >= min.y && x < max.x && y < max.y;
    }

    template <typename DownEvent, typename UpEvent>
    ButtonElement<DownEvent, UpEvent>::ButtonElement(float x, float y, float width, float height)
    : BoxElement(x, y, width, height) {}

    template <typename DownEvent, typename UpEvent>
    entt::entity ButtonElement<DownEvent, UpEvent>::register_with(entt::registry& registry) {
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
        return entt::null;
    }

    template <typename DownEvent, typename UpEvent>
    void ButtonElement<DownEvent, UpEvent>::deregister(entt::registry& registry) {
        registry.destroy(m_entity);
    }

    template <typename DownEvent, typename UpEvent>
    void ButtonElement<DownEvent, UpEvent>::handle(MouseButtonEvent& event, InterfaceView& emitter) {
        if (down) {
            down = event.pressed;
            if(!down)
                emitter.publish<UpEvent>(build_up_event(event, emitter));
        } else if(collides(event.x, event.y)) {
            down = event.pressed;
            if(down)
                emitter.publish<DownEvent>(build_down_event(event, emitter));
        }
    }

    template <typename DownEvent, typename UpEvent>
    entt::entity ButtonElement<DownEvent, UpEvent>::get_entity() {
        return m_entity;
    }

    template <typename DownEvent, typename UpEvent>
    TexturedButton<DownEvent, UpEvent>::TexturedButton(std::string up_texture, std::string down_texture,
                                                       float x, float y, float width, float height)
            : ButtonElement<DownEvent, UpEvent>(x, y, width, height),
                    unclicked_tex_name(std::move(up_texture)),
                    clicked_tex_name(std::move(down_texture)) {}

    template <typename DownEvent, typename UpEvent>
    entt::entity TexturedButton<DownEvent, UpEvent>::register_with(entt::registry& registry) {
        TextureSprite sprite;
        sprite.name = unclicked_tex_name;
        sprite.vertices.emplace_back(this->min.x, this->min.y);
        sprite.uvs.emplace_back(0,0);

        sprite.vertices.emplace_back(this->min.x, this->max.y);
        sprite.uvs.emplace_back(0,1);

        sprite.vertices.emplace_back(this->max.x, this->max.y);
        sprite.uvs.emplace_back(1,1);

        sprite.vertices.emplace_back(this->max.x, this->min.y);
        sprite.uvs.emplace_back(1,0);

        sprite.indices.push_back(0);
        sprite.indices.push_back(1);
        sprite.indices.push_back(2);
        sprite.indices.push_back(2);
        sprite.indices.push_back(3);
        sprite.indices.push_back(0);

        if(this->m_entity == entt::null)
            this->m_entity = registry.create();
        registry.emplace_or_replace<TextureSprite>(this->m_entity, sprite);
        registry.patch<InstanceList>(this->m_entity, [](auto &instance_list) {
            instance_list.add_instance(glm::mat4(1));
        });
        return entt::null;
    }

    StartButton::StartButton(const RenderContext& ctx) :
            TexturedButton<TexSwapEvent, StartEvent>(
                    "/Users/philipsmith/Downloads/kenny1/2D assets/UI Base Pack/PNG/blue_button00.png",
                    "/Users/philipsmith/Downloads/kenny1/2D assets/UI Base Pack/PNG/blue_button01.png",
                    ctx.screen_width / 2 - 85, 20, 190, 49) {}

    StartEvent StartButton::build_up_event(MouseButtonEvent& event, InterfaceView& emitter) {
        return StartEvent{
            m_entity,
            unclicked_tex_name
        };
    }

    TexSwapEvent StartButton::build_down_event(MouseButtonEvent& event, InterfaceView& emitter) {
        return TexSwapEvent{
            m_entity,
            clicked_tex_name
        };
    }

    ExitButton::ExitButton(const RenderContext& ctx) :
    TexturedButton<TexSwapEvent, ExitEvent>(
            "/Users/philipsmith/Downloads/kenny1/2D assets/UI Base Pack/PNG/red_button01.png",
            "/Users/philipsmith/Downloads/kenny1/2D assets/UI Base Pack/PNG/red_button00.png",
            ctx.screen_width / 2 - 85, 100, 190, 49) {}

    ExitEvent ExitButton::build_up_event(MouseButtonEvent& event, InterfaceView& emitter) {
        return ExitEvent{
            m_entity,
            unclicked_tex_name
        };
    }

    TexSwapEvent ExitButton::build_down_event(MouseButtonEvent& event, InterfaceView& emitter) {
        return TexSwapEvent{
            m_entity,
            clicked_tex_name
        };
    }
} // namespace engine
