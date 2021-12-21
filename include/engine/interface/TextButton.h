//
// Created by Philip Smith on 11/9/21.
//

#ifndef ENGINE_TEXTBUTTON_H
#define ENGINE_TEXTBUTTON_H

#include <engine/sprite/TextSprite.h>
#include <utility>

#include "TexturedButton.h"


namespace engine {
	template <typename Event>
	class TextButton : public TexturedButton<Event> {
	public:
		TextButton(std::string font,
				   std::string text,
				   float x, float y,
				   float width, float height,
				   std::string up_texture, std::string down_texture)
		: TexturedButton<Event>(x, y, width, height, std::move(up_texture), std::move(down_texture)),
		        m_font(std::move(font)), m_text(std::move(text)){}

		entt::entity register_with(entt::registry& registry) override {
			auto entity = TexturedButton<Event>::register_with(registry);
			registry.emplace_or_replace<TextSprite>(entity, m_font, m_text, this->min.x + 10, this->max.y - 10, 1.f, glm::vec3(1,1,1));
			return entity;
		}
	protected:
		std::string m_font, m_text;
	};

}

#endif //ENGINE_TEXTBUTTON_H
