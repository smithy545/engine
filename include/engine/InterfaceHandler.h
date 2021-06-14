//
// Created by Philip Smith on 5/25/21.
//

#ifndef ENGINE_INTERFACEHANDLER_H
#define ENGINE_INTERFACEHANDLER_H

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <string>
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
            public RegistryHandler,
            public KeyHandler,
            public MouseButtonHandler,
            public MouseMotionHandler {
    public:
        InterfaceHandler(InterfaceContainer::Ptr container, entt::registry &registry);

        void update(entt::registry &registry) override;

    private:
        static constexpr entt::hashed_string SHAPES_KEY{"shapes"};
        InterfaceContainer::Ptr m_container;

        // mouse motion
        bool trigger(double x, double y) override;

        // mouse press
        bool trigger(double x, double y, int button, bool value) override;

        // key press
        bool trigger(int code, bool value) override;
    };
} // namespace engine


#endif //ENGINE_INTERFACEHANDLER_H
