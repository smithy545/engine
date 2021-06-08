//
// Created by Philip Smith on 5/25/21.
//

#ifndef ENGINE_INTERFACEHANDLER_H
#define ENGINE_INTERFACEHANDLER_H


#include <cute_c2.h>
#include <engine/Sprite.h>
#include <entt/entt.hpp>
#include <fmt/format.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <utils/file_util.h>
#include <vector>

#include "InterfaceContainer.h"
#include "KeyHandler.h"
#include "MouseButtonHandler.h"
#include "MouseMotionHandler.h"
#include "RegistryHandler.h"


namespace engine {
    class InterfaceHandler :
            public RegistryHandler, public KeyHandler, public MouseButtonHandler, public MouseMotionHandler {
    public:
        explicit InterfaceHandler(entt::registry &registry);

        void update(entt::registry &registry) override;

    private:
        // TODO: find way to avoid hardcoding schema key name here and in schema file separately
        static constexpr entt::hashed_string SHAPES_KEY{"shapes"};
        InterfaceContainer::Ptr m_container;

        static void load_interface(const json& interface, entt::registry &registry);

        // mouse motion
        bool trigger(double x, double y) override;

        // mouse press
        bool trigger(double x, double y, int button, bool value) override;

        // key press
        bool trigger(int code, bool value) override;
    };
} // namespace engine


#endif //ENGINE_INTERFACEHANDLER_H
