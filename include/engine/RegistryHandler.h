//
// Created by Philip Smith on 5/25/21.
//

#ifndef ENGINE_STATEHANDLER_H
#define ENGINE_STATEHANDLER_H

#include <entt/entt.hpp>


namespace engine {
    class RegistryHandler {
    public:
        virtual void update(entt::registry &registry) = 0;
    };
} // namespace engine

#endif //ENGINE_STATEHANDLER_H
