//
// Created by Philip Smith on 5/25/21.
//

#include <engine/InterfaceHandler.h>

#include <engine/InputHandler.h>
#include <engine/InstanceList.h>
#include <engine/Sprite.h>
#include <vector>


namespace engine {
    InterfaceHandler::InterfaceHandler(entt::registry &registry) {
        std::vector<glm::vec2> vertices{
            glm::vec2(0,0),
            glm::vec2(0, 100),
            glm::vec2(100, 0)
        };
        auto white = glm::vec3(1,1,1);
        std::vector<glm::vec3> colors{
            white, white, white
        };
        std::vector<unsigned int> indices{0,1,2};
        auto entity = registry.create();
        registry.emplace<Sprite>(entity, vertices, colors, indices);
        registry.patch<InstanceList>(entity, [](auto &instance_list) {
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
} // namespace engine
