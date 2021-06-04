//
// Created by Philip Smith on 5/25/21.
//

#ifndef ENGINE_UIHANDLER_H
#define ENGINE_UIHANDLER_H

#include <engine/Sprite.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <utils/file_util.h>
#include <vector>

#include "StateHandler.h"
#include "KeyHandler.h"
#include "MouseButtonHandler.h"
#include "MouseMotionHandler.h"


namespace engine {
    class InterfaceHandler : public StateHandler, public KeyHandler, public MouseButtonHandler, public MouseMotionHandler {
    public:
        explicit InterfaceHandler(entt::registry &registry);

        void update(entt::registry &registry) override;

    private:
        // TODO: find way to avoid hardcoding schema key name here and in schema file separately
        static constexpr entt::hashed_string NAME_KEY{"name"};
        static constexpr entt::hashed_string SHAPES_KEY{"shapes"};
        static constexpr entt::hashed_string TRANSITIONS_KEY{"transitions"};

        static void load_interface(const json& interface, entt::registry &registry);

        static void load_shape_from_json(const json& shape, entt::registry &registry);

        // mouse motion
        bool trigger(double x, double y) override;

        // mouse press
        bool trigger(double x, double y, int button, bool value) override;

        // key press
        bool trigger(int code, bool value) override;
    };
} // namespace engine


#endif //ENGINE_UIHANDLER_H
