//
// Created by Philip Smith on 6/9/21.
//

#include <engine/ButtonElement.h>

#include <engine/InstanceList.h>
#include <engine/Sprite.h>
#include <iostream>
#include <utility>


namespace engine {
    ButtonElement::ButtonElement(float x, float y, float width, float height, std::string text)
    : BoxElement(x, y, width, height), m_text(std::move(text)) {}

    void ButtonElement::register_with(entt::registry& registry) {
        Sprite sprite;
        sprite.vertices.emplace_back(min.x, min.y);
        sprite.colors.emplace_back(1,1,1);

        sprite.vertices.emplace_back(min.x, max.y);
        sprite.colors.emplace_back(1,1,1);

        sprite.vertices.emplace_back(max.x, max.y);
        sprite.colors.emplace_back(1,1,1);

        sprite.vertices.emplace_back(max.x, min.y);
        sprite.colors.emplace_back(1,1,1);

        sprite.indices.push_back(0);
        sprite.indices.push_back(1);
        sprite.indices.push_back(2);
        sprite.indices.push_back(2);
        sprite.indices.push_back(3);
        sprite.indices.push_back(0);

        if(entity == entt::null)
            entity = registry.create();
        registry.emplace_or_replace<Sprite>(entity, sprite);
        registry.patch<InstanceList>(entity, [](auto &instance_list) {
            instance_list.set_models({glm::mat4(1)});
        });
    }

    void ButtonElement::deregister(entt::registry& registry) {
        registry.destroy(entity);
    }

    bool ButtonElement::handle_mouse_move(double x, double y) {
        return false;
    }

    bool ButtonElement::handle_mouse_down(double x, double y, int button) {
        return false;
    }

    bool ButtonElement::handle_mouse_up(double x, double y, int button) {
        return false;
    }

    bool ButtonElement::handle_key_down(int code) {
        return false;
    }

    bool ButtonElement::handle_key_up(int code) {
        return false;
    }
} // namespace engine
