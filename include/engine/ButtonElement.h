//
// Created by Philip Smith on 6/9/21.
//

#ifndef CIVILWAR_BUTTONELEMENT_H
#define CIVILWAR_BUTTONELEMENT_H

#include "BoxElement.h"

#include <string>
#include <utils/macros.h>


namespace engine {
    class ButtonElement : public BoxElement {
    public:
        PTR(ButtonElement);

        ButtonElement(float x, float y, float width, float height, std::string text);

        void register_with(entt::registry& registry) override;

        void deregister(entt::registry& registry) override;

        bool handle_mouse_move(double x, double y) override;

        bool handle_mouse_down(double x, double y, int button) override;

        bool handle_mouse_up(double x, double y, int button) override;

        bool handle_key_down(int code) override;

        bool handle_key_up(int code) override;
    private:
        entt::entity entity{entt::null};
    REFVAR(std::string, text, public, private);
    };
} // namespace engine


#endif //CIVILWAR_BUTTONELEMENT_H
