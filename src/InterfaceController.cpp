//
// Created by Philip Smith on 5/25/21.
//

#include <engine/InterfaceController.h>

#include <engine/RenderContext.h>
#include <engine/GameManager.h>

#include <utility>


using namespace entt::literals;
namespace fs = std::filesystem;

namespace engine {
    InterfaceController::InterfaceController(entt::registry &registry, Renderer &renderer)
    : IndependentEntity(registry), renderer(renderer), context(renderer.get_context(registry)) {}

    void InterfaceController::update() {
        if(m_state != nullptr)
            m_state->update(context);
        if(m_prev_state != nullptr)
            m_prev_state = nullptr;
    }

    void InterfaceController::set_state(InterfaceView::Ptr state) {
        m_prev_state = std::move(m_state);
        m_state = std::move(state);
        m_state->load(context);
    }

    void InterfaceController::set_camera(Camera::Ptr camera) {
        renderer.set_camera(registry, std::move(camera));
    }

    bool InterfaceController::trigger(double scroll_delta) {
        m_state->publish<MouseScrollEvent>(scroll_delta);
        return true;
    }

    bool InterfaceController::trigger(double x, double y) {
        m_state->publish<MouseMotionEvent>(x, y);
        return true;
    }

    bool InterfaceController::trigger(double x, double y, int button, bool value) {
        m_state->publish<MouseButtonEvent>(x, y, button, value);
        return true;
    } 

    bool InterfaceController::trigger(int code, bool value) {
        m_state->publish<KeyEvent>(code, value);
        return true;
    }
} // namespace engine
