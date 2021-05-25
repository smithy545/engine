//
// Created by Philip Smith on 5/25/21.
//

#ifndef ENGINE_UIHANDLER_H
#define ENGINE_UIHANDLER_H

#include <entt/entt.hpp>

#include "StateHandler.h"


namespace engine {
    class UIHandler : public StateHandler {
    public:
        void update(entt::registry &registry) override;
    };
} // namespace engine


#endif //ENGINE_UIHANDLER_H
