//
// Created by Philip Smith on 11/10/21.
//

#ifndef ENGINE_BOXCONTAINER_H
#define ENGINE_BOXCONTAINER_H

#include "BoxElement.h"
#include <engine/InstanceList.h>
#include <engine/ManagedEntity.h>
#include <engine/sprite/ShapeSprite.h>
#include <vector>


namespace engine {
	class BoxContainer : public BoxElement, public ManagedEntity {
	public:
		PTR(BoxContainer);

		BoxContainer(float x, float y, float width, float height, float padding, glm::vec3 color = glm::vec3(0.5, 0.5, 0.5));

		entt::entity register_with(entt::registry& registry) override {
			ShapeSprite sprite;
			sprite.vertices.emplace_back(min.x, min.y);
			sprite.colors.push_back(m_color);

			sprite.vertices.emplace_back(min.x, max.y);
			sprite.colors.push_back(m_color);

			sprite.vertices.emplace_back(max.x, max.y);
			sprite.colors.push_back(m_color);

			sprite.vertices.emplace_back(max.x, min.y);
			sprite.colors.push_back(m_color);

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

		entt::entity get_entity() override {
			return m_entity;
		}
	private:
		entt::entity m_entity{entt::null};
		float m_padding;
		glm::vec3 m_color;
		std::vector<InterfaceElement::Ptr> m_children;
	};
} // namespace engine

#endif //ENGINE_BOXCONTAINER_H
