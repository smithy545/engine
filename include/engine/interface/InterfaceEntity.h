//
// Created by Philip Smith on 12/27/21.
//

#ifndef ENGINE_INTERFACEENTITY_H
#define ENGINE_INTERFACEENTITY_H

#include <engine/entity.h>
#include <utils/macros.h>
#include <utils/math_util.h>


using utils::math::Point_2;

namespace engine {
	struct InterfaceEntity : public EntityObject {
		PTR(InterfaceEntity);

		Point_2 pos;
		Ptr next{nullptr};

		explicit InterfaceEntity(Point_2 pos) : pos(pos) {}

		InterfaceEntity(Point_2 pos, entt::entity entity) : pos(pos) {
			m_entity = entity;
		}

		entt::entity register_with(entt::registry &registry) override {
			if (m_entity == entt::null)
				m_entity = registry.create();
			return m_entity;
		}
	};
} // namespace engine

#endif //ENGINE_INTERFACEENTITY_H
