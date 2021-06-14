//
// Created by Philip Smith on 6/13/21.
//

#ifndef CIVILWAR_MANAGEDENTITY_H
#define CIVILWAR_MANAGEDENTITY_H

#include <entt/entt.hpp>


namespace engine {
    struct ManagedEntity {
        virtual void register_with(entt::registry& registry) = 0;

        virtual void deregister(entt::registry& registry) = 0;
    };
} // namespace engine


#endif //CIVILWAR_MANAGEDENTITY_H
