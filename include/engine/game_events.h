//
// Created by Philip Smith on 6/21/21.
//

#ifndef ENGINE_GAME_EVENTS_H
#define ENGINE_GAME_EVENTS_H

#include <entt/entt.hpp>
#include <string>


namespace engine {
    struct TexSwapEvent {
        entt::entity entity;
        std::string texture_name;
    };
    struct StartEvent : public TexSwapEvent {};
    struct ExitEvent : public TexSwapEvent {};
} // namespace engine

#endif //ENGINE_GAME_EVENTS_H
