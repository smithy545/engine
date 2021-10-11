//
// Created by Philip Smith on 7/15/21.
//

#ifndef ENGINE_TEXTUREBUTTONELEMENT_H
#define ENGINE_TEXTUREBUTTONELEMENT_H

#include "ButtonBase.h"

#include <engine/game_events.h>
#include <engine/InstanceList.h>
#include <engine/sprite/TextureSprite.h>
#include <string>


namespace engine {
	struct NoopEvent {};

	template <typename ActivationEvent, typename DownEvent = NoopEvent>
	class TexturedButton : public ButtonBase<DownEvent, ActivationEvent> {
	public:
		PTR(TexturedButton);

		// TODO: automatically set width and height according to textures
		TexturedButton(float x, float y, float width, float height, std::string up_texture, std::string down_texture)
		: ButtonBase<DownEvent, ActivationEvent>(x, y, width, height),
		m_default_tex_name(std::move(up_texture)),
		m_activated_tex_name(std::move(down_texture)) {}

		entt::entity register_with(entt::registry& registry) override {
			TextureSprite sprite;
			sprite.name = m_default_tex_name;
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

		ActivationEvent* build_up_event(MouseButtonEvent& event, InterfaceView& emitter) override {
			emitter.get_registry().patch<TextureSprite>(this->m_entity, [&](TextureSprite& sprite) {
				sprite.name = m_default_tex_name;
			});
			return new ActivationEvent{};
		}

		DownEvent* build_down_event(MouseButtonEvent& event, InterfaceView& emitter) override {
			emitter.get_registry().patch<TextureSprite>(this->m_entity, [&](TextureSprite& sprite) {
				sprite.name = m_activated_tex_name;
			});
			return new DownEvent{};
		}
	protected:
		std::string m_default_tex_name;
		std::string m_activated_tex_name;
	};
} // namespace engine


#endif //ENGINE_TEXTUREBUTTONELEMENT_H
