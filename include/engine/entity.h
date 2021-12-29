//
// Created by Philip Smith on 12/22/21.
//

#ifndef ENGINE_ENTITY_H
#define ENGINE_ENTITY_H

#include <entt/entt.hpp>


namespace engine {
	struct ManagedEntity {
		virtual entt::entity register_with(entt::registry& registry) = 0;

		virtual void deregister(entt::registry& registry) {
			registry.destroy(get_entity());
		}

		virtual entt::entity get_entity() = 0;
	};

	struct EntityObject : ManagedEntity {
		entt::entity get_entity() override {
			return m_entity;
		}
	protected:
		entt::entity m_entity{entt::null};
	};

	class IndependentEntity {
	public:
		explicit IndependentEntity(entt::registry& registry) : registry(registry) {};
	protected:
		entt::registry& registry;
	};
} // namespace engine

#endif //ENGINE_ENTITY_H
