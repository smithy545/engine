//
// Created by Philip Smith on 6/9/21.
//

#ifndef CIVILWAR_BUTTONELEMENT_H
#define CIVILWAR_BUTTONELEMENT_H

#include "BoxElement.h"

#include <string>
#include <utils/macros.h>

#include "input_events.h"
#include "ManagedEntity.h"


namespace engine {
    class ButtonElement : public BoxElement, public ManagedEntity, public MouseButtonEventSink {
    public:
        PTR(ButtonElement);

        ButtonElement(float x, float y, float width, float height, std::string text);

        void register_with(entt::registry& registry) override;

        void deregister(entt::registry& registry) override;

        void handle(MouseButtonEvent& event, InterfaceContainer& emitter) override;
    private:
        entt::entity entity{entt::null};
    REFVAR(std::string, text, public, private);
    };
} // namespace engine


#endif //CIVILWAR_BUTTONELEMENT_H
