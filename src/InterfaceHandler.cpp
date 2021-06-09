//
// Created by Philip Smith on 5/25/21.
//

#include <engine/InterfaceHandler.h>

#include <engine/Camera.h>
#include <engine/InputManager.h>
#include <utility>


using namespace entt::literals;
namespace fs = std::filesystem;

namespace engine {
    InterfaceHandler::InterfaceHandler(InterfaceContainer::Ptr container, entt::registry &registry)
    : m_container(std::move(container)) {
        m_container->register_with(registry);
    }

    void InterfaceHandler::update(entt::registry &registry) {
        auto view = registry.view<Camera>();
        for(auto &entity: view) {
            auto &camera = view.get<Camera>(entity);
            if(!camera.filming)
                continue;
            if (InputManager::get_key(GLFW_KEY_SPACE))
                InputManager::is_paused() ? InputManager::unpause() : InputManager::pause();
            if (InputManager::get_key(GLFW_KEY_W))
                camera.move_forward();
            if (InputManager::get_key(GLFW_KEY_A))
                camera.move_left();
            if (InputManager::get_key(GLFW_KEY_S))
                camera.move_backward();
            if (InputManager::get_key(GLFW_KEY_D))
                camera.move_right();
            if (InputManager::get_key(GLFW_KEY_LEFT_SHIFT))
                camera.move_up();
            if (InputManager::get_key(GLFW_KEY_LEFT_CONTROL))
                camera.move_down();
            if (InputManager::get_mouse_x() != InputManager::get_prev_mouse_x())
                ;//camera.pan_horizontal(InputManager::get_prev_mouse_x() - InputManager::get_mouse_x());
            if (InputManager::get_mouse_y() != InputManager::get_prev_mouse_y())
                ;//camera.pan_vertical(InputManager::get_mouse_y() - InputManager::get_prev_mouse_y());
        }
    }

    bool InterfaceHandler::trigger(double x, double y) {
        return m_container->handle_mouse_move(x, y);
    }

    bool InterfaceHandler::trigger(double x, double y, int button, bool value) {
        if(value)
            return m_container->handle_mouse_down(x, y, button);
        return m_container->handle_mouse_up(x, y, button);
    }

    bool InterfaceHandler::trigger(int code, bool value) {
        if(value)
            return m_container->handle_key_down(code);
        return m_container->handle_key_up(code);
    }
} // namespace engine
