//
// Created by Philip Smith on 7/15/21.
//

#ifndef ENGINE_TEXTUREBUTTONELEMENT_H
#define ENGINE_TEXTUREBUTTONELEMENT_H

#include "ButtonElement.h"
#include "game_events.h"
#include "InstanceList.h"
#include "sprite.h"
#include <string>


namespace engine {
    template <typename DownEvent, typename UpEvent>
    class TextureButtonElement : public ButtonElement<DownEvent, UpEvent> {
    public:
        PTR(TextureButtonElement);

        static_assert(std::is_base_of<TexSwapEvent, UpEvent>::value, "Texture button events must extend TexSwapEvent");
        static_assert(std::is_base_of<TexSwapEvent, DownEvent>::value, "Texture button events must extend TexSwapEvent");

        // TODO: automatically set width and height according to textures
        TextureButtonElement(std::string up_texture, std::string down_texture,
                             float x, float y, float width, float height)
                : ButtonElement<DownEvent, UpEvent>(x, y, width, height),
                  unclicked_tex_name(std::move(up_texture)),
                  clicked_tex_name(std::move(down_texture)) {}

        entt::entity register_with(entt::registry& registry) override {
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

        UpEvent* build_up_event(MouseButtonEvent& event, InterfaceView& emitter) override {
            TexSwapEvent* outgoing_event = new UpEvent{};
            outgoing_event->texture_name = unclicked_tex_name;
            outgoing_event->entity = this->m_entity;
            return static_cast<UpEvent*>(outgoing_event);
        }

        DownEvent* build_down_event(MouseButtonEvent& event, InterfaceView& emitter) override {
            TexSwapEvent* outgoing_event = new DownEvent{};
            outgoing_event->texture_name = clicked_tex_name;
            outgoing_event->entity = this->m_entity;
            return static_cast<DownEvent*>(outgoing_event);
        }
    protected:
        std::string unclicked_tex_name;
        std::string clicked_tex_name;
    };
} // namespace engine


#endif //ENGINE_TEXTUREBUTTONELEMENT_H
