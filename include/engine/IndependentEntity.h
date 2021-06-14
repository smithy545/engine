//
// Created by Philip Smith on 6/13/21.
//

#ifndef CIVILWAR_INDEPENDENTENTITY_H
#define CIVILWAR_INDEPENDENTENTITY_H

#include <entt/entt.hpp>


namespace engine {
    class IndependentEntity {
    public:
        explicit IndependentEntity(entt::registry& registry) : registry(registry) {};
    protected:
        entt::registry& registry;
    };
} // namespace engine


#endif //CIVILWAR_INDEPENDENTENTITY_H
