//
// Created by Philip Smith on 5/25/21.
//

#include <engine/InterfaceHandler.h>

#include <engine/OrbitCam.h>
#include <engine/InputManager.h>
#include <utility>


using namespace entt::literals;
namespace fs = std::filesystem;

namespace engine {
    InterfaceHandler::InterfaceHandler(InterfaceContainer::Ptr container, entt::registry &registry)
    : m_container(std::move(container)) {}

    void InterfaceHandler::update(entt::registry &registry) {
        if(m_container != nullptr)
            m_container->update();

        auto view = registry.view<OrbitCam>();
        for(auto &entity: view) {
            auto &camera = view.get<OrbitCam>(entity);
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
            if (InputManager::get_key(GLFW_KEY_Q))
                camera.pan_horizontal(0.1);
            if (InputManager::get_key(GLFW_KEY_E))
                camera.pan_horizontal(-0.1);
        }
    }

    bool InterfaceHandler::trigger(double x, double y) {
        if(m_container->collides(x, y)) {
            m_container->publish<MouseMotionEvent>(x, y);
            return false;
        }
        return true;
    }

    bool InterfaceHandler::trigger(double x, double y, int button, bool value) {
        if(m_container->collides(x, y)) {
            m_container->publish<MouseButtonEvent>(x, y, button, value);
            return false;
        }
        return true;
    }

    bool InterfaceHandler::trigger(int code, bool value) {
        m_container->publish<KeyEvent>(code, value);
        return false;
    }
} // namespace engine
