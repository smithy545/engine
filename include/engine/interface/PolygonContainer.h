//
// Created by Philip Smith on 12/14/21.
//

#ifndef ENGINE_POLYGONCONTAINER_H
#define ENGINE_POLYGONCONTAINER_H

#include "ShapeComponent.h"

#include <engine/InstanceList.h>
#include <engine/interface/InterfaceEntity.h>
#include <engine/sprite/ShapeSprite.h>
#include <glm/gtx/transform.hpp>
#include <vector>


namespace engine {
	class PolygonContainer : public ShapeComponent, public InterfaceEntity {
	public:
		PTR(PolygonContainer);

		PolygonContainer(float x, float y, const std::vector<glm::vec2>& vertices, glm::vec3 color = glm::vec3(.5, .5, .5))
		: ShapeElement(x, y, vertices), m_color(color) {}

		entt::entity register_with(entt::registry& registry) override {
			ShapeSprite sprite;
			for(auto v: verts) {
				sprite.vertices.emplace_back(v.x - c.x, v.y - c.y);
				sprite.colors.push_back(m_color);
			}
			for(auto i = 1; i < count - 1; i++) {
				sprite.indices.push_back(0);
				sprite.indices.push_back(i+1);
				sprite.indices.push_back(i);
			}

			if(m_entity == entt::null)
				m_entity = registry.create();
			registry.emplace_or_replace<ShapeSprite>(m_entity, sprite);
			registry.patch<InstanceList>(m_entity, [&](auto &instance_list) {
				instance_list.add_instance(glm::translate(glm::mat4(1), glm::vec3(c.x, c.y, 0)));
			});
			return m_entity;
		}
	private:
		glm::vec3 m_color;
	};
} // namespace engine

#endif //ENGINE_POLYGONCONTAINER_H
