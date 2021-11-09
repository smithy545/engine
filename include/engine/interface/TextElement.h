//
// Created by Philip Smith on 7/15/21.
//

#ifndef ENGINE_TEXTELEMENT_H
#define ENGINE_TEXTELEMENT_H

#include "InterfaceElement.h"
#include <engine/ManagedEntity.h>
#include <engine/sprite/TextSprite.h>
#include <utils/macros.h>
#include <string>


namespace engine {
	class TextElement : public InterfaceElement, public ManagedEntity {
	public:
		PTR(TextElement);

		explicit TextElement(std::string font, std::string text, float x = 0, float y = 0, float scale = 1, glm::vec3 color = glm::vec3(1,1,1))
		: m_font(std::move(font)), m_text(std::move(text)), m_pos(x, y), m_scale(scale), m_color(color) {}

		entt::entity register_with(entt::registry& registry) override {
			if(m_entity == entt::null)
				m_entity = registry.create();
			registry.emplace_or_replace<TextSprite>(m_entity, m_font, m_text, m_pos.x, m_pos.y, m_scale, m_color);
			return m_entity;
		}

		entt::entity get_entity() override {
			return m_entity;
		}

		glm::vec2 get_center() override {
			return m_pos;
		}
	private:
		entt::entity m_entity{entt::null};
		std::string m_font, m_text;
		glm::vec3 m_color;
		glm::vec2 m_pos;
		float m_scale;
	};
} // namespace engine


#endif //ENGINE_TEXTELEMENT_H
