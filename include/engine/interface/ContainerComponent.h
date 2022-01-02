//
// Created by Philip Smith on 1/2/22.
//

#ifndef ENGINE_CONTAINERCOMPONENT_H
#define ENGINE_CONTAINERCOMPONENT_H

#include <set>

#include "InterfaceEntity.h"
#include "InterfaceView.h"


namespace engine {
	class ContainerComponent {
	public:
		explicit ContainerComponent(InterfaceView& view_ref) : m_parent(view_ref) {}

		void add_child(const InterfaceEntity::Ptr& child) {
			m_children.insert(m_parent.insert_element(child));
		}
	private:
		InterfaceView& m_parent;
		std::set<entt::entity> m_children;
	};
} // namespace engine


#endif //ENGINE_CONTAINERCOMPONENT_H
