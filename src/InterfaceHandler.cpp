//
// Created by Philip Smith on 5/25/21.
//

#include <engine/InterfaceHandler.h>

#include <engine/OrbitCam.h>
#include <engine/Steadicam.h>
#include <engine/RenderContext.h>
#include <engine/GameManager.h>
#include <utility>


using namespace entt::literals;
namespace fs = std::filesystem;

namespace engine {
    InterfaceHandler::InterfaceHandler(InterfaceContainer::Ptr container, entt::registry &registry)
    : IndependentEntity(registry), m_container(std::move(container)) {}

    void InterfaceHandler::update() {
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
                if (GameManager::get_key(GLFW_KEY_SPACE))
                    GameManager::is_paused() ? GameManager::unpause() : GameManager::pause();
                if (GameManager::get_key(GLFW_KEY_W))
                    orbit_cam->move_forward();
                if (GameManager::get_key(GLFW_KEY_A))
                    orbit_cam->move_left();
                if (GameManager::get_key(GLFW_KEY_S))
                    orbit_cam->move_backward();
                if (GameManager::get_key(GLFW_KEY_D))
                    orbit_cam->move_right();
                if (GameManager::get_key(GLFW_KEY_LEFT_SHIFT))
                    orbit_cam->move_up();
                if (GameManager::get_key(GLFW_KEY_LEFT_CONTROL))
                    orbit_cam->move_down();
                if (GameManager::get_key(GLFW_KEY_Q))
                    orbit_cam->pan_horizontal(0.1);
                if (GameManager::get_key(GLFW_KEY_E))
                    orbit_cam->pan_horizontal(-0.1);
            } else if(auto* steadicam = dynamic_cast<Steadicam*>(camera.get())) {
                // TODO: First person mode
            }
        }
    }

    bool InterfaceHandler::trigger(double scroll_delta) {
        m_container->publish<MouseScrollEvent>(scroll_delta);
        return true;
    }

    bool InterfaceHandler::trigger(double x, double y) {
        m_container->publish<MouseMotionEvent>(x, y);
        return true;
    }

    bool InterfaceHandler::trigger(double x, double y, int button, bool value) {
        m_container->publish<MouseButtonEvent>(x, y, button, value);
        return true;
    }

    bool InterfaceHandler::trigger(int code, bool value) {
        m_container->publish<KeyEvent>(code, value);
        return true;
    }
} // namespace engine
