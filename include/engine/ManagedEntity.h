//
// Created by Philip Smith on 6/13/21.
//

#ifndef ENGINE_MANAGEDENTITY_H
#define ENGINE_MANAGEDENTITY_H

#include <entt/entt.hpp>


namespace engine {
    struct ManagedEntity {
        virtual entt::entity register_with(entt::registry& registry) = 0;

        virtual void deregister(entt::registry& registry) = 0;

        virtual entt::entity get_entity() = 0;
    };
} // namespace engine


#endif //ENGINE_MANAGEDENTITY_H
