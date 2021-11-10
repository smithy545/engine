//
// Created by Philip Smith on 6/13/21.
//

#ifndef ENGINE_INDEPENDENTENTITY_H
#define ENGINE_INDEPENDENTENTITY_H

#include <entt/entt.hpp>


namespace engine {
    class IndependentEntity {
    public:
        explicit IndependentEntity(entt::registry& registry) : registry(registry) {};
    protected:
        entt::registry& registry;
    };
} // namespace engine


#endif //ENGINE_INDEPENDENTENTITY_H
