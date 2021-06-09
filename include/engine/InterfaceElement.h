//
// Created by Philip Smith on 6/7/21.
//

#ifndef ENGINE_INTERFACEELEMENT_H
#define ENGINE_INTERFACEELEMENT_H

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <utils/macros.h>
#include <vector>


namespace engine {
    class InterfaceElement {
    public:
        PTR(InterfaceElement);

        friend class InterfaceContainer;

        virtual void register_with(entt::registry& registry) = 0;

        virtual void deregister(entt::registry& registry) = 0;

        virtual bool handle_mouse_move(double x, double y) = 0;

        virtual bool handle_mouse_down(double x, double y, int button) = 0;

        virtual bool handle_mouse_up(double x, double y, int button) = 0;

        virtual bool handle_key_down(int code) = 0;

        virtual bool handle_key_up(int code) = 0;

        virtual glm::vec2 get_center() = 0;

        virtual bool collides(double x, double y) {
            return false;
        }
    REFVAR(Ptr, next, public, protected){nullptr};
    };
} // namespace engine


#endif //ENGINE_INTERFACEELEMENT_H
