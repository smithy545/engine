//
// Created by Philip Smith on 5/25/21.
//

#include <engine/InterfaceHandler.h>

#include <engine/InputHandler.h>
#include <engine/InstanceList.h>
#include <utils/math_util.h>


using namespace entt::literals;

namespace engine {
    InterfaceHandler::InterfaceHandler(entt::registry &registry) {
        auto red_box = generate_colored_polygon({
            glm::vec2(200, 200),
            glm::vec2(200, 400),
            glm::vec2(400, 400),
            glm::vec2(400, 200)
        }, glm::vec3(1,0,0));
        auto box_entity = registry.create();
        registry.emplace<Sprite>(box_entity, red_box);
        registry.patch<InstanceList>(box_entity, [](auto &instance_list) {
            instance_list.set_models(std::vector<glm::mat4>{
                    glm::mat4(1)
            });
        });
    }

    void InterfaceHandler::update(entt::registry &registry) {
        if (InputHandler::get_key(GLFW_KEY_W))
            ;//camera.move_forward();
        if (InputHandler::get_key(GLFW_KEY_A))
            ;//camera.move_left();
        if (InputHandler::get_key(GLFW_KEY_S))
            ;//camera.move_backward();
        if (InputHandler::get_key(GLFW_KEY_D))
            ;//camera.move_right();
        if (InputHandler::get_key(GLFW_KEY_SPACE))
            ;//is_paused() ? unpause() : pause();
        if (InputHandler::get_key(GLFW_KEY_LEFT_SHIFT))
            ;//camera.move_up();
        if (InputHandler::get_key(GLFW_KEY_LEFT_CONTROL))
            ;//camera.move_down();
        if (InputHandler::get_mouse_x() != InputHandler::get_prev_mouse_x())
            ;//camera.pan_horizontal(InputHandler::get_prev_mouse_x() - InputHandler::get_mouse_x());
        if (InputHandler::get_mouse_y() != InputHandler::get_prev_mouse_y())
            ;//camera.pan_vertical(InputHandler::get_mouse_y() - InputHandler::get_prev_mouse_y());
    }

    Sprite InterfaceHandler::generate_colored_polygon(const std::vector<glm::vec2>& hull_points, glm::vec3 color) {
        std::vector<double> coords;
        for(auto v: hull_points) {
            coords.push_back(v.x);
            coords.push_back(v.y);
        }
        auto verts = utils::math::triangulate(coords);
        std::vector<glm::vec3> colors;
        std::vector<unsigned int> indices;
        for(int i = 0; i < verts.size(); i++) {
            colors.push_back(color);
            indices.push_back(i);
        }
        return Sprite{verts, colors, indices};
    }
} // namespace engine
