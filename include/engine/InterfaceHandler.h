//
// Created by Philip Smith on 5/25/21.
//

#ifndef ENGINE_UIHANDLER_H
#define ENGINE_UIHANDLER_H

#include <engine/Sprite.h>
#include <entt/entt.hpp>
#include <utils/file_util.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

#include "StateHandler.h"


namespace engine {
    class InterfaceHandler : public StateHandler {
    public:
        explicit InterfaceHandler(entt::registry &registry);

        void update(entt::registry &registry) override;

    private:
        std::unordered_map<std::string, json> interfaces;

        static Sprite generate_colored_polygon(const std::vector<glm::vec2>& hull_points, glm::vec3 color);
    };
} // namespace engine


#endif //ENGINE_UIHANDLER_H
