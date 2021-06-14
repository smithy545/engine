//
// Created by Philip Smith on 5/25/21.
//

#include <engine/InterfaceHandler.h>

#include <engine/OrbitCam.h>
#include <engine/Steadicam.h>
#include <engine/RenderContext.h>
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

        auto view = registry.view<RenderContext>();
        for(auto &entity: view) {
            auto [context] = view.get(entity);
            auto camera = context.camera;
            if(auto* orbit_cam = dynamic_cast<OrbitCam*>(camera.get())){
                // STRATEGY MODE
                if(!orbit_cam->filming)
                    continue;
                if (InputManager::get_key(GLFW_KEY_SPACE))
                    InputManager::is_paused() ? InputManager::unpause() : InputManager::pause();
                if (InputManager::get_key(GLFW_KEY_W))
                    orbit_cam->move_forward();
                if (InputManager::get_key(GLFW_KEY_A))
                    orbit_cam->move_left();
                if (InputManager::get_key(GLFW_KEY_S))
                    orbit_cam->move_backward();
                if (InputManager::get_key(GLFW_KEY_D))
                    orbit_cam->move_right();
                if (InputManager::get_key(GLFW_KEY_LEFT_SHIFT))
                    orbit_cam->move_up();
                if (InputManager::get_key(GLFW_KEY_LEFT_CONTROL))
                    orbit_cam->move_down();
                if (InputManager::get_key(GLFW_KEY_Q))
                    orbit_cam->pan_horizontal(0.1);
                if (InputManager::get_key(GLFW_KEY_E))
                    orbit_cam->pan_horizontal(-0.1);
            } else if(auto* steadicam = dynamic_cast<Steadicam*>(camera.get())) {
                // TODO: First person mode
            }
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
