//
// Created by Philip Smith on 6/8/21.
//
#define CUTE_C2_IMPLEMENTATION
#include <engine/BoxElement.h>

#include <engine/InstanceList.h>
#include <engine/Sprite.h>


namespace engine {
    BoxElement::BoxElement(float x, float y, float width, float height)
    : c2AABB({x, y, x+width, y+height}), m_center((min.x+max.x)/2.0, (min.y+max.y)/2.0) {}

    glm::vec2 BoxElement::get_center() {
        return m_center;
    }

    bool BoxElement::collides(double x, double y) {
        return x >= min.x && y >= min.y && x < max.x && y < max.y;
    }

    ColoredBoxElement::ColoredBoxElement(float x, float y, float width, float height, glm::vec3 color)
    : BoxElement(x, y, width, height), m_color(color) {}

    void ColoredBoxElement::register_with(entt::registry& registry) {
        Sprite sprite;
        sprite.vertices.emplace_back(min.x, min.y);
        sprite.colors.push_back(m_color);

        sprite.vertices.emplace_back(min.x, max.y);
        sprite.colors.push_back(m_color);

        sprite.vertices.emplace_back(max.x, max.y);
        sprite.colors.push_back(m_color);

        sprite.vertices.emplace_back(max.x, min.y);
        sprite.colors.push_back(m_color);

        sprite.indices.push_back(0);
        sprite.indices.push_back(1);
        sprite.indices.push_back(2);
        sprite.indices.push_back(2);
        sprite.indices.push_back(3);
        sprite.indices.push_back(0);

        entity = registry.create();
        registry.emplace<Sprite>(entity, sprite);
        registry.patch<InstanceList>(entity, [](auto &instance_list) {
            instance_list.set_models({glm::mat4(1)});
        });
    }

    void ColoredBoxElement::deregister(entt::registry& registry) {
        registry.destroy(entity);
        entity = entt::null;
    }

    bool ColoredBoxElement::handle_mouse_move(double x, double y) {
        return false;
    }

    bool ColoredBoxElement::handle_mouse_down(double x, double y, int button) {
        return false;
    }

    bool ColoredBoxElement::handle_mouse_up(double x, double y, int button) {
        return false;
    }

    bool ColoredBoxElement::handle_key_down(int code) {
        return false;
    }

    bool ColoredBoxElement::handle_key_up(int code) {
        return false;
    }
} // namespace engine
